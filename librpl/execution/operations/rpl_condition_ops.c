//
//  rpl_condition_ops.c
//  librpl
//
//  Created by Chris Hanson on 10/17/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_condition_ops_internal.h"

#include <assert.h>

#include "rpl_interpreter_internal.h"

#include "rpl_name.h"
#include "rpl_identifier.h"
#include "rpl_condition_internal.h"


RPL_SOURCE_BEGIN


rpl_op_definition_t rpl_condition_op_defs[] = {
{"_CONDITION", rpl_operation_type_command,rpl_condition_op__CONDITION},
{"_PROTECT",   rpl_operation_type_command,rpl_condition_op__PROTECT},
{"CONRCL",     rpl_operation_type_function,rpl_condition_op_CONRCL},
{"RESUME",     rpl_operation_type_command,rpl_condition_op_RESUME},
{"RETRY",      rpl_operation_type_command,rpl_condition_op_RETRY},
{"ABORT",      rpl_operation_type_command,rpl_condition_op_ABORT},
{NULL, 0, NULL },
};


bool
rpl_configure_condition_ops(rpl_operation_table_t table)
{
    assert(table != NULL);

    return rpl_operations_register_defs(table, rpl_condition_op_defs);
}


/* MARK: - Condition System Operations */

/*!
 Implementation of the `_CONDITION` operation, which creates a condition
 value and pushes it on the stack. It takes as its arguments the number
 of key/value pairs to put in the condition's payload, the key/value
 pairs themselves, and the name of the condition to add them as payload
 to, and returns the condition itself.
 */
bool
rpl_condition_op__CONDITION(rpl_operation_t operation,
			    rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

    rpl_stack_t stack = rpl_context_get_stack(context);
    assert(stack != NULL);

    rpl_interpreter_t interp = rpl_context_get_interpreter(context);
    assert(interp != NULL);

    rpl_value_t condition = NULL;
    rpl_value_t kvcv = NULL;
    rpl_value_t vv = NULL;
    rpl_value_t kv = NULL;
    rpl_value_t name = NULL;
    bool did_set = false;

    /* Create the condition in advance of adding things to it. */

    condition = rpl_condition_new();
    if (condition == NULL) goto error;

    /* Validate number of arguments. */

    const rpl_integer_t level = rpl_stack_get_level(stack);
    if (level < 2) {
	// TODO: Signal 'insufficient arguments' condition
	goto error;
    }

    /*
     Pop the value then key pairs off the stack, and add them to the
     condition created above.
     */

    kvcv = rpl_stack_pop(stack);
    assert(kvcv != NULL);
    if (rpl_value_get_type(kvcv) != rpl_type_integer) {
	// TODO: Signal 'invalid type' condition
	goto error;
    }

    const rpl_integer_t kvc = rpl_integer_get_rep(kvcv);
    assert(level <= (1 + (kvc * 2) + 1));

    for (rpl_integer_t p = 0; p < kvc; p++) {

	/* Pop the value, which can be any type. */

	vv = rpl_stack_pop(stack);
	assert(vv != NULL);

	/* Pop the key, which must be a name or identifier. */

	kv = rpl_stack_pop(stack);
	assert(kv != NULL);

	const rpl_type_t kt = rpl_value_get_type(kv);
	if ((kt != rpl_type_name) && (kt != rpl_type_identifier)) {
	    // TODO: Signal 'invalid type' condition
	    goto error;
	}

	/* Set the payload value. */

	did_set = rpl_condition_set_payload_value(condition, kv, vv);
	if (did_set == false) {
	    // TODO: Signal 'memory full' condition
	    goto error;
	}

	/* Clean up. */

	rpl_value_release(vv); vv = NULL;
	rpl_value_release(kv); kv = NULL;
    }

    /* Pop the condition's name, which must be a name or identifier. */

    name = rpl_stack_pop(stack);
    assert(name != NULL);

    rpl_unistring_t names = NULL;
    rpl_type_t namet = rpl_value_get_type(name);
    if (namet == rpl_type_name) {
	names = rpl_name_get_rep(name);
    } else if (namet == rpl_type_identifier) {
	names = rpl_identifier_get_rep(name);
    } else {
	// TODO: Signal 'invalid type' condition
	goto error;
    }

    did_set = rpl_condition_set_name(condition, names);
    if (did_set == false) {
	// TODO: Signal 'memory full' condition
	goto error;
    }

    /* Push the fully-constructed condition object on the stack. */

    rpl_stack_push(stack, condition);

    /* Clean up. */

    rpl_value_release(condition);
    rpl_value_release(kvcv);
    rpl_value_release(name);

    return true;

error:
    if (condition) rpl_value_release(condition);
    if (kvcv) rpl_value_release(kvcv);
    if (vv) rpl_value_release(vv);
    if (kv) rpl_value_release(kv);
    if (name) rpl_value_release(name);
    return false;
}

bool
rpl_condition_op__PROTECT(rpl_operation_t operation,
			  rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

    rpl_stack_t stack = rpl_context_get_stack(context);
    assert(stack != NULL);

    rpl_interpreter_t interp = rpl_context_get_interpreter(context);
    assert(interp != NULL);

    // TODO: Implement _PROTECT

    return true;

error:
    return false;
}

bool
rpl_condition_op_CONRCL(rpl_operation_t operation,
			rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

    rpl_stack_t stack = rpl_context_get_stack(context);
    assert(stack != NULL);

    rpl_interpreter_t interp = rpl_context_get_interpreter(context);
    assert(interp != NULL);

    rpl_value_t condition = NULL;
    rpl_value_t key = NULL;
    rpl_value_t value = NULL;

    /* Validate number of arguments. */

    const rpl_integer_t level = rpl_stack_get_level(stack);
    if (level < 2) {
	// TODO: Signal 'insufficient arguments' condition
	goto error;
    }

    /* Pop the name to recall off the stack. */

    key = rpl_stack_pop(stack);
    assert(key != NULL);

    if ((rpl_value_get_type(key) != rpl_type_name)
	|| (rpl_value_get_type(key) == rpl_type_identifier))
    {
	// TODO: Signal 'invalid type' error
	goto error;
    }

    /* Pop the condition to recall from off the stack. */

    condition = rpl_stack_pop(stack);
    assert(condition != NULL);

    if (rpl_value_get_type(condition) != rpl_type_condition) {
	// TODO: Signal 'invalid type' error
	goto error;
    }

    /* Get the value from the condition and push it on the stack. */

    value = rpl_condition_get_payload_value(condition, key);
    if (value == NULL) {
	// TODO: Signal 'invalid key' error
	goto error;
    }

    rpl_stack_push(stack, value);

    /* Clean up and return. */

    rpl_value_release(condition);
    rpl_value_release(key);
    rpl_value_release(value);

    return true;

error:
    if (condition) rpl_value_release(condition);
    if (key) rpl_value_release(key);
    if (value) rpl_value_release(value);
    return false;
}

bool
rpl_condition_op_RESUME(rpl_operation_t operation,
			rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

    rpl_stack_t stack = rpl_context_get_stack(context);
    assert(stack != NULL);

    rpl_interpreter_t interp = rpl_context_get_interpreter(context);
    assert(interp != NULL);

    // TODO: Implement RESUME

    return true;

error:
    return false;
}

bool
rpl_condition_op_RETRY(rpl_operation_t operation,
		       rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

    rpl_stack_t stack = rpl_context_get_stack(context);
    assert(stack != NULL);

    rpl_interpreter_t interp = rpl_context_get_interpreter(context);
    assert(interp != NULL);

    // TODO: Implement RETRY

    return true;

error:
    return false;
}

bool
rpl_condition_op_ABORT(rpl_operation_t operation,
		       rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

    rpl_stack_t stack = rpl_context_get_stack(context);
    assert(stack != NULL);

    rpl_interpreter_t interp = rpl_context_get_interpreter(context);
    assert(interp != NULL);

    // TODO: Implement ABORT

    return true;

error:
    return false;
}


RPL_SOURCE_END
