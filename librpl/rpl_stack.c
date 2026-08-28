//
//  rpl_stack.c
//  librpl
//
//  Created by Chris Hanson on 8/28/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_stack_internal.h"

#include <assert.h>
#include <stdlib.h>


RPL_SOURCE_BEGIN


rpl_stack_t RPL_NULLABLE
rpl_stack_new(rpl_integer_t depth)
{
    assert(depth > 0);

    rpl_stack_t stack = calloc(1, sizeof(struct rpl_stack));
    if (stack) {
	stack->_depth = depth;
	stack->_values = calloc(depth, sizeof(rpl_value_t));
	if (stack->_values == NULL) goto error;
	stack->_sp = depth;
    }
    return stack;

error:
    rpl_stack_free(stack);
    return NULL;
}


void
rpl_stack_free(rpl_stack_t stack)
{
    assert(stack != NULL);

    free(stack->_values);
    free(stack);
}


rpl_integer_t
rpl_stack_get_depth(rpl_stack_t stack)
{
    assert(stack != NULL);

    return stack->_depth;
}


rpl_integer_t
rpl_stack_get_sp(rpl_stack_t stack)
{
    assert(stack != NULL);

    return stack->_sp;
}


void
rpl_stack_push(rpl_stack_t stack, rpl_value_t value)
{
    assert(stack != NULL);
    assert(value != NULL);

    assert(stack->_sp > 0);
    assert(stack->_sp <= stack->_depth);

    stack->_sp -= 1;
    stack->_values[stack->_sp] = value;
}


rpl_value_t RPL_NULLABLE
rpl_stack_pop(rpl_stack_t stack)
{
    rpl_value_t result = NULL;

    assert(stack != NULL);
    assert(stack->_sp >= 0);

    if (stack->_sp < stack->_depth) {
	result = stack->_values[stack->_sp];
	stack->_sp += 1;
    }

    return result;
}


RPL_SOURCE_END
