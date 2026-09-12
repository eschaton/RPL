//
//  rpl_stack_ops.c
//  librpl
//
//  Created by Chris Hanson on 9/11/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_stack_ops_internal.h"

#include <assert.h>
#include <string.h>


RPL_SOURCE_BEGIN


struct rpl_stack_op_entry {
    const char *_name;
    rpl_operation_type_t _type;
    rpl_operation_impl_t _impl;
} rpl_stack_op_entries[] = {
    { "DUP", rpl_operation_type_function, rpl_stack_op_dup },
    { "DROP", rpl_operation_type_function, rpl_stack_op_drop },
    { "SWAP", rpl_operation_type_function, rpl_stack_op_swap },
    { NULL, 0, NULL },
};


bool
rpl_configure_stack_ops(rpl_operation_table_t table)
{
    assert(table != NULL);

    bool added;

    rpl_unistring_t name = NULL;
    rpl_operation_t op = NULL;

    for (struct rpl_stack_op_entry *entry = &rpl_stack_op_entries[0];
	 (entry->_name != NULL);
	 entry++)
    {
	const size_t name_len = strlen(entry->_name);
	assert(name_len != 0);

	name = rpl_unistring_new_from_utf8(entry->_name, name_len);
	if (name == NULL) goto error;

	op = rpl_operation_new(name, entry->_type, entry->_impl, NULL);
	if (op == NULL) goto error;

	rpl_unistring_release(name); name = NULL;

	added = rpl_operation_table_set(table, op);
	if (added == false) goto error;
    }

    return true;

error:
    if (name) rpl_unistring_release(name);
    if (op) rpl_operation_free(op);
    return false;
}


/* MARK: - Stack Operations */

bool
rpl_stack_op_dup(rpl_operation_t operation,
		 rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

    rpl_stack_t stack = rpl_context_get_stack(context);
    assert(stack != NULL);

    rpl_value_t value = NULL;
    rpl_value_t copy = NULL;

    if (rpl_stack_get_level(stack) >= 1) {
	value = rpl_stack_pop(stack);
	assert(value != NULL);

	rpl_stack_push(stack, value);

	copy = rpl_value_copy(value);
	if (copy == NULL) goto error;
	// TODO: Signal 'allocation failure' condition

	rpl_stack_push(stack, copy);

	rpl_value_release(value);
	rpl_value_release(copy);
    } else {
	// TODO: Signal 'stack is empty' condition
	goto error;
    }

    return true;

error:
    if (value) rpl_value_release(value);
    if (copy) rpl_value_release(copy);
    return false;
}

bool
rpl_stack_op_drop(rpl_operation_t operation,
		  rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

    rpl_stack_t stack = rpl_context_get_stack(context);
    assert(stack != NULL);

    if (rpl_stack_get_level(stack) >= 1) {
	rpl_value_t value = rpl_stack_pop(stack);
	assert(value != NULL);

	rpl_value_release(value);
    } else {
	// TODO: Signal 'stack is empty' condition
	goto error;
    }

    return true;

error:
    return false;
}

bool
rpl_stack_op_swap(rpl_operation_t operation,
		  rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

    rpl_stack_t stack = rpl_context_get_stack(context);
    assert(stack != NULL);

    rpl_value_t a = NULL;
    rpl_value_t b = NULL;

    if (rpl_stack_get_level(stack) >= 2) {
	a = rpl_stack_pop(stack);
	assert(a != NULL);

	b = rpl_stack_pop(stack);
	assert(b != NULL);

	rpl_stack_push(stack, a);
	rpl_stack_push(stack, b);

	rpl_value_release(a);
	rpl_value_release(b);
    } else {
	// TODO: Signal 'stack is empty' condition
	goto error;
    }

    return true;

error:
    return false;
}


RPL_SOURCE_END
