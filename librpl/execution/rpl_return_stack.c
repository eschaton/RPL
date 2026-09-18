//
//  rpl_return_stack.c
//  librpl
//
//  Created by Chris Hanson on 9/18/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_return_stack_internal.h"

#include <assert.h>
#include <stdlib.h>

#include "rpl_return.h"


RPL_SOURCE_BEGIN


rpl_return_stack_t RPL_NULLABLE
rpl_return_stack_new(rpl_integer_t depth)
{
    assert(depth > 0);
    
    rpl_return_stack_t stack
	= calloc(1, sizeof(struct rpl_return_stack));
    if (stack) {
	stack->_returns = calloc(depth, sizeof(rpl_return_t));
	if (stack->_returns == NULL) goto error;
	stack->_depth = depth;
	stack->_level = 0;
    }
    return stack;

error:
    rpl_return_stack_free(stack);
    return NULL;
}

void
rpl_return_stack_free(rpl_return_stack_t stack)
{
    assert(stack != NULL);

    if (stack->_returns) {
	for (rpl_integer_t i = 0; i < stack->_depth; i++) {
	    rpl_return_t ret = stack->_returns[i];
	    if (ret) rpl_return_free(ret);
	}
    }

    free(stack->_returns);
    free(stack);
}

rpl_integer_t
rpl_return_stack_get_depth(rpl_return_stack_t stack)
{
    assert(stack != NULL);

    return stack->_depth;
}

rpl_integer_t
rpl_return_stack_get_level(rpl_return_stack_t stack)
{
    assert(stack != NULL);

    return stack->_level;
}

void
rpl_return_stack_push(rpl_return_stack_t stack, rpl_return_t ret)
{
    assert(stack != NULL);
    assert(ret != NULL);

    assert(stack->_level < (stack->_depth - 1));

    stack->_returns[stack->_level] = ret;
    stack->_level += 1;
}

rpl_return_t
rpl_return_stack_pop(rpl_return_stack_t stack)
{
    assert(stack != NULL);
    assert(stack->_level > 0);

    stack->_level -= 1;
    rpl_return_t result = stack->_returns[stack->_level];

    /*
     Since freeing a stack frees any returns remaining on it, popping a
     return must set that slot to NULL to avoid an over-free.
     */
    stack->_returns[stack->_level] = NULL;

    return result;
}


RPL_SOURCE_END
