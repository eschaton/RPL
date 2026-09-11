//
//  rpl_interpreter.c
//  librpl
//
//  Created by Chris Hanson on 9/10/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_interpreter_internal.h"

#include <assert.h>
#include <stdlib.h>

#include "rpl_name.h"
#include "rpl_operation.h"


RPL_SOURCE_BEGIN


rpl_interpreter_t RPL_NULLABLE
rpl_interpreter_new(void)
{
    rpl_interpreter_t interp
	= calloc(1, sizeof(struct rpl_interpreter));
    if (interp) {
	interp->_context = rpl_context_new();
	if (interp->_context == NULL) goto error;

	interp->_tokenizer = rpl_tokenizer_new(interp->_context);
	if (interp->_tokenizer == NULL) goto error;

	interp->_optable = rpl_operation_table_new();
	if (interp->_optable == NULL) goto error;

	interp->_global = rpl_scope_new(interp->_local);
	if (interp->_global == NULL) goto error;

	interp->_local = rpl_scope_new(interp->_global);
	if (interp->_local == NULL) goto error;

	interp->_output = rpl_unistring_new(80 * 24);
	if (interp->_output == NULL) goto error;
    }
    return interp;

error:
    rpl_interpreter_free(interp);
    return NULL;
}

RPL_EXPORT
void
rpl_interpreter_free(rpl_interpreter_t interp)
{
    assert(interp != NULL);

    if (interp->_context) rpl_context_free(interp->_context);
    if (interp->_tokenizer) rpl_tokenizer_free(interp->_tokenizer);
    if (interp->_global) rpl_scope_free(interp->_global);
    if (interp->_local) rpl_scope_free(interp->_local);

    free(interp);
}

RPL_EXPORT
bool
rpl_interpreter_append_input(rpl_interpreter_t interp,
			     rpl_unistring_t str)
{
    assert(interp != NULL);
    assert(str != NULL);

    return rpl_tokenizer_append(interp->_tokenizer, str);
}

RPL_EXPORT
bool
rpl_interpreter_has_output(rpl_interpreter_t interp)
{
    assert(interp != NULL);

    return rpl_unistring_get_length(interp->_output) > 0;
}

rpl_unistring_t RPL_NULLABLE
rpl_interpreter_copy_output(rpl_interpreter_t interp)
{
    assert(interp != NULL);

    rpl_unistring_t output = rpl_unistring_copy(interp->_output);
    if (output) {
	rpl_unistring_remove_all(interp->_output);
    }

    return output;
}

bool
rpl_interpreter_step(rpl_interpreter_t interp)
{
    assert(interp != NULL);

    bool success = false;

    rpl_token_t token = rpl_tokenizer_copy_next(interp->_tokenizer);
    if (token) {
	success = rpl_interpreter_eval(interp, token);
	rpl_token_free(token);
    }

    bool appended;
    rpl_unistring_t s = NULL;
    rpl_unistring_t eol = NULL;

    /* Dump the stack to the output. */

    if (success) {
	rpl_stack_t stack = rpl_context_get_stack(interp->_context);
	assert(stack != NULL);

	rpl_environment_t env
	    = rpl_context_get_environment(interp->_context);
	assert(env != NULL);

	const rpl_integer_t depth = rpl_stack_get_depth(stack);
	if (depth > 0) {
	    eol = rpl_unistring_new_from_utf8("\n", 1);
	    if (eol == NULL) goto error;

	    for (rpl_integer_t i = depth; i > 0; --i) {
		rpl_value_t v = rpl_stack_get_value_at_level(stack, i);
		assert(v != NULL);

		s = rpl_value_copy_string(v, env);
		if (s == NULL) goto error;

		appended = rpl_unistring_append(interp->_output, s);
		if (appended == false) goto error;
		rpl_unistring_release(s); s = NULL;

		appended = rpl_unistring_append(interp->_output, eol);
		if (appended == false) goto error;
	    }

	    rpl_unistring_release(eol);
	}
    }

error:
    if (eol) rpl_unistring_release(eol);
    if (s) rpl_unistring_release(s);

    return success;
}


bool
rpl_interpreter_eval(rpl_interpreter_t interp, rpl_token_t token)
{
    assert(interp != NULL);
    assert(token != NULL);

    bool success = false;

    rpl_token_type_t token_type = rpl_token_get_type(token);
    if (token_type == rpl_token_type_value) {
	rpl_value_t value = rpl_token_get_value(token);
	rpl_value_retain(value);

	rpl_stack_t stack = rpl_context_get_stack(interp->_context);
	rpl_stack_push(stack, value);
	rpl_value_release(value);

	success = true;
    } else if (token_type == rpl_token_type_identifier) {
	rpl_unistring_t identifier = rpl_token_get_string(token);
	rpl_unistring_retain(identifier);

	rpl_stack_t stack = rpl_context_get_stack(interp->_context);

	/*
	 If the identifier is in the operation table, get and invoke the
	 associated operation.
	 */

	rpl_operation_t op = rpl_operation_table_get(interp->_optable,
						     identifier);
	if (op) {
	    bool opsuccess = rpl_operation_invoke(op, interp->_context);
	    success = opsuccess;
	    goto done;
	}

	/* If the identifier is in the local scope, push its value. */

	rpl_value_t lv = rpl_scope_get_variable(interp->_local,
						identifier);
	if (lv) {
	    rpl_stack_push(stack, lv);
	    success = true;
	    goto done;
	}

	/* If the identifier is in the global scope, push its value. */

	rpl_value_t gv = rpl_scope_get_variable(interp->_global,
						identifier);
	if (gv) {
	    rpl_stack_push(stack, gv);
	    success = true;
	    goto done;
	}

	/* Create a name from the identifier and push it. */

	rpl_value_t name = rpl_name_new(identifier);
	if (name) {
	    rpl_stack_push(stack, name);
	    rpl_value_release(name);
	    success = true;
	    goto done;
	} else {
	    success = false;
	    goto done;
	}

    done:
	rpl_unistring_release(identifier);
    } else {
	/* Should never happen. */
	assert((token_type == rpl_token_type_value)
	       || token_type == rpl_token_type_identifier);
    }

    return success;
}


RPL_SOURCE_END
