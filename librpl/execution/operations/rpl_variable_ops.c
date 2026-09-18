//
//  rpl_variable_ops.c
//  RPL
//
//  Created by Chris Hanson on 9/17/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_variable_ops_internal.h"

#include <assert.h>

#include "rpl_name.h"
#include "rpl_operations_internal.h"


RPL_SOURCE_BEGIN


rpl_op_definition_t rpl_variable_op_defs[] = {
    { "RCL", rpl_operation_type_function, rpl_variable_op_RCL },
    { "STO", rpl_operation_type_command, rpl_variable_op_STO },
    { NULL, 0, NULL },
};


bool
rpl_configure_variable_ops(rpl_operation_table_t table)
{
    assert(table != NULL);

    return rpl_operations_register_defs(table, rpl_variable_op_defs);
}


/* MARK: - Variable Operations */

bool
rpl_variable_op_RCL(rpl_operation_t operation,
		    rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

    rpl_stack_t stack = rpl_context_get_stack(context);
    assert(stack != NULL);

    rpl_value_t name_val = NULL;
    rpl_unistring_t name = NULL;
    rpl_value_t value = NULL;

    if (rpl_stack_get_level(stack) >= 1) {
	name_val = rpl_stack_pop(stack);
	assert(name_val != NULL);

	// TODO: Signal 'not a name' condition
	if (rpl_value_get_type(name_val) != rpl_type_name) goto error;

	name = rpl_name_get_rep(name_val);
	assert(name != NULL);

	rpl_scope_t locals = rpl_context_get_local_scope(context);
	assert(locals != NULL);

	value = rpl_scope_get_variable(locals, name, true);
	// TODO: Signal 'no value for variable' condition
	if (value == NULL) goto error;

	rpl_stack_push(stack, value);

	rpl_value_release(name_val); name_val = NULL;
	rpl_value_release(value); value = NULL;
    } else {
	// TODO: Signal 'stack is empty' condition
	goto error;
    }

    return true;

error:
    if (name_val) rpl_value_release(name_val);
    if (value) rpl_value_release(value);
    return false;
}

bool
rpl_variable_op_STO(rpl_operation_t operation,
		    rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

    rpl_stack_t stack = rpl_context_get_stack(context);
    assert(stack != NULL);

    rpl_value_t name_val = NULL;
    rpl_unistring_t name = NULL;
    rpl_value_t value = NULL;

    if (rpl_stack_get_level(stack) >= 2) {
	name_val = rpl_stack_pop(stack);
	assert(name_val != NULL);

	// TODO: Signal 'not a name' condition
	if (rpl_value_get_type(name_val) != rpl_type_name) goto error;

	name = rpl_name_get_rep(name_val);
	assert(name != NULL);

	value = rpl_stack_pop(stack);
	assert(value != NULL);

	rpl_scope_t locals = rpl_context_get_local_scope(context);
	assert(locals != NULL);

	bool did_set = rpl_scope_set_variable(locals, name, value);
	// TODO: Signal 'couldn't set variable' condition
	if (did_set == false) goto error;

	rpl_value_release(name_val); name_val = NULL;
	rpl_value_release(value); value = NULL;
    } else {
	// TODO: Signal 'stack is empty' condition
	goto error;
    }

    return true;

error:
    if (name_val) rpl_value_release(name_val);
    if (value) rpl_value_release(value);
    return false;
}


RPL_SOURCE_END
