//
//  rpl_return_stack.h
//  librpl
//
//  Created by Chris Hanson on 9/18/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_return_stack__h__
#define __RPL__rpl_return_stack__h__

#include "rpl_defines.h"

#include "rpl_integer.h"


RPL_HEADER_BEGIN


/* Forward declaration to avoid circular inclusion. */
typedef struct rpl_return *rpl_return_t;


/*! The stack of "returns" used to implement control flow. */
typedef struct rpl_return_stack *rpl_return_stack_t;


RPL_EXPORT
rpl_return_stack_t RPL_NULLABLE
rpl_return_stack_new(rpl_integer_t depth);

RPL_EXPORT
void
rpl_return_stack_free(rpl_return_stack_t stack);

RPL_EXPORT
rpl_integer_t
rpl_return_stack_get_depth(rpl_return_stack_t stack);

RPL_EXPORT
rpl_integer_t
rpl_return_stack_get_level(rpl_return_stack_t stack);

RPL_EXPORT
void
rpl_return_stack_push(rpl_return_stack_t stack, rpl_return_t ret);

RPL_EXPORT
rpl_return_t
rpl_return_stack_pop(rpl_return_stack_t stack);

/*! Get the topmost open return on the return stack. */
RPL_EXPORT
rpl_return_t RPL_NULLABLE
rpl_return_stack_get_top_open(rpl_return_stack_t stack);


RPL_HEADER_END


#endif /* __RPL__rpl_return_stack__h__ */
