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
rpl_interpreter_new(rpl_configure_interpreter_f configurator,
		    void * RPL_NULLABLE refcon)
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

	interp->_output = rpl_unistring_new(80 * 24);
	if (interp->_output == NULL) goto error;

	bool configured = (*configurator)(interp, refcon);
	if (configured == false) goto error;
    }
    return interp;

error:
    rpl_interpreter_free(interp);
    return NULL;
}

void
rpl_interpreter_free(rpl_interpreter_t interp)
{
    assert(interp != NULL);

    if (interp->_context) rpl_context_free(interp->_context);
    if (interp->_tokenizer) rpl_tokenizer_free(interp->_tokenizer);
    if (interp->_output) rpl_unistring_release(interp->_output);

    free(interp);
}

rpl_context_t
rpl_interpreter_get_context(rpl_interpreter_t interp)
{
    assert(interp != NULL);

    return interp->_context;
}

bool
rpl_interpreter_append_input(rpl_interpreter_t interp,
			     rpl_unistring_t str)
{
    assert(interp != NULL);
    assert(str != NULL);

    return rpl_tokenizer_append(interp->_tokenizer, str);
}

bool
rpl_interpreter_has_output(rpl_interpreter_t interp)
{
    assert(interp != NULL);

    return rpl_unistring_get_length(interp->_output) > 0;
}

rpl_unistring_t RPL_NULLABLE
rpl_interpreter_copy_output(rpl_interpreter_t interp)
RPL_RETURNS_RETAINED
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

    return success;
}

bool
rpl_interpreter_output_stack(rpl_interpreter_t interp)
{
    assert(interp != NULL);

    bool appended;
    rpl_unistring_t s = NULL;

    rpl_context_t context = interp->_context;

    rpl_stack_t stack = rpl_context_get_stack(context);
    assert(stack != NULL);

    rpl_environment_t env = rpl_context_get_environment(context);
    assert(env != NULL);

    const rpl_integer_t depth = rpl_stack_get_depth(stack);
    if (depth > 0) {
	rpl_unistring_t eol = rpl_unistring_get_eol();

	for (rpl_integer_t i = depth; i > 0; --i) {
	    rpl_value_t v = rpl_stack_get_value_at_level(stack, i);
	    assert(v != NULL);

	    s = rpl_unistring_new(80);
	    if (s == NULL) goto error;

	    rpl_unistring_t is = rpl_unistring_with_integer(i);
	    if (is == NULL) goto error;
	    appended = rpl_unistring_append(s, is);
	    rpl_unistring_release(is);
	    if (appended == false) goto error;

	    appended = rpl_unistring_append_char(s, ':');
	    if (appended == false) goto error;

	    appended = rpl_unistring_append_char(s, ' ');
	    if (appended == false) goto error;

	    rpl_unistring_t vs = rpl_value_copy_string(v, env);
	    if (vs == NULL) goto error;
	    appended = rpl_unistring_append(s, vs);
	    rpl_unistring_release(vs);
	    if (appended == false) goto error;

	    appended = rpl_unistring_append(s, eol);
	    if (appended == false) goto error;

	    appended = rpl_unistring_append(interp->_output, s);
	    if (appended == false) goto error;

	    rpl_unistring_release(s); s = NULL;
	}
    }

    return true;

error:
    if (s) rpl_unistring_release(s);

    return false;
}

bool
rpl_interpreter_eval_value(rpl_interpreter_t interp, rpl_value_t value)
{
    assert(interp != NULL);
    assert(value != NULL);

    /*
     At this level, evaluating a value just involves pushing it on the
     stack.
     */

    rpl_stack_t stack = rpl_context_get_stack(interp->_context);
    rpl_stack_push(stack, value);

    /* The stack takes ownership of the value. */

    rpl_value_release(value);

    return true;
}

bool
rpl_interpreter_eval_identifier(rpl_interpreter_t interp,
				rpl_unistring_t identifier)
{
    assert(interp != NULL);
    assert(identifier != NULL);

    bool success = false;

    /*
     If the identifier is in the operation table, get and invoke the
     associated operation, and return whether its invocation succeeded.
     */

    rpl_operation_t op = rpl_operation_table_get(interp->_optable,
						 identifier);
    if (op) {
	success = rpl_operation_invoke(op, interp->_context);
	goto done;
    }

    /*
     If the identifier represents a variable in the local or global
     scope, evaluate its value the same way a direct value would be.
     */
    rpl_scope_t scope = rpl_context_get_local_scope(interp->_context);
    assert(scope != NULL);

    rpl_value_t value = rpl_scope_get_variable(scope, identifier, true);
    if (value) {
	success = rpl_interpreter_eval_value(interp, value);
	goto done;
    }

    /* Create a name from the identifier, and treat that as a value. */

    rpl_value_t name = rpl_name_new(identifier);
    if (name) {
	success = rpl_interpreter_eval_value(interp, name);
	goto done;
    } else {
	success = false;
	goto done;
    }

done:
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
	success = rpl_interpreter_eval_value(interp, value);
    } else if (token_type == rpl_token_type_identifier) {
	rpl_unistring_t identifier = rpl_token_get_string(token);

	success = rpl_interpreter_eval_identifier(interp, identifier);
    } else {
	/* Should never happen. */
	assert((token_type == rpl_token_type_value)
	       || token_type == rpl_token_type_identifier);
    }

    return success;
}


RPL_SOURCE_END
