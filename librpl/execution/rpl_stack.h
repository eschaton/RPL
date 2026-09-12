//
//  rpl_stack.h
//  librpl
//
//  Created by Chris Hanson on 8/28/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_stack__h__
#define __RPL__rpl_stack__h__

#include "rpl_defines.h"

#include "rpl_integer.h"
#include "rpl_value.h"


RPL_HEADER_BEGIN


/*! A stack of RPL values. */
typedef struct rpl_stack *rpl_stack_t;


/*! Create a stack of a given initial depth. */
RPL_EXPORT
rpl_stack_t RPL_NULLABLE
rpl_stack_new(rpl_integer_t depth);

/*! Dispose of a stack. */
RPL_EXPORT
void
rpl_stack_free(rpl_stack_t stack);

/*! Get the current depth of the stack. */
RPL_EXPORT
rpl_integer_t
rpl_stack_get_depth(rpl_stack_t stack);

/*! Get the current stack level. */
RPL_EXPORT
rpl_integer_t
rpl_stack_get_level(rpl_stack_t stack);

/*! Push a value onto the stack. */
RPL_EXPORT
void
rpl_stack_push(rpl_stack_t stack, rpl_value_t value);

/*! Pop and return the value at the top of the stack. */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_stack_pop(rpl_stack_t stack)
RPL_RETURNS_RETAINED;

/*!
 Get the value at the given level.

 - WARNING: This should only really be used for display and debugging.
 */
RPL_EXPORT
rpl_value_t
rpl_stack_get_value_at_level(rpl_stack_t stack, rpl_integer_t level);


RPL_HEADER_END

#endif /* __RPL__rpl_stack__h__ */
