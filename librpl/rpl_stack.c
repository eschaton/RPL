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

#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN


/*
 RPL implements a stack as growing up, not down, because the direction
 of the growth is immaterial to the behavior and growing upwards makes
 the code slightly more straightforward.
 */


rpl_stack_t RPL_NULLABLE
rpl_stack_new(rpl_integer_t depth)
{
    assert(depth > 0);

    rpl_stack_t stack = calloc(1, sizeof(struct rpl_stack));
    if (stack) {
	stack->_values = calloc(depth, sizeof(rpl_value_t));
	if (stack->_values == NULL) goto error;
	stack->_level = 0;
	stack->_depth = depth;
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

    if (stack->_values) {
	rpl_value_release_array(stack->_values, stack->_depth);
    }

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
rpl_stack_get_level(rpl_stack_t stack)
{
    assert(stack != NULL);

    return stack->_level;
}

void
rpl_stack_push(rpl_stack_t stack, rpl_value_t value)
{
    assert(stack != NULL);
    assert(value != NULL);

    assert(stack->_level < (stack->_depth - 1));

    stack->_values[stack->_level] = rpl_value_retain(value);
    stack->_level += 1;
}

rpl_value_t RPL_NULLABLE
rpl_stack_pop(rpl_stack_t stack)
{
    assert(stack != NULL);
    assert(stack->_level > 0);

    stack->_level -= 1;
    rpl_value_t result = stack->_values[stack->_level];

    /*
     Since freeing a stack releases any objects remaining on it, popping
     a value must set that slot to NULL to avoid an over-release.
     */
    stack->_values[stack->_level] = NULL;

    return result;
}

void
rpl_stack_drop(rpl_stack_t stack)
{
    assert(stack != NULL);
    assert(stack->_level > 0);

    rpl_value_t popped = rpl_stack_pop(stack);

    rpl_value_release(popped);
}

rpl_value_t
rpl_stack_get_value_at_level(rpl_stack_t stack, rpl_integer_t level)
{
    assert(stack != NULL);
    assert(level < stack->_level);

    rpl_value_t value = stack->_values[level];
    assert(value != NULL);

    return value;
}


RPL_SOURCE_END
