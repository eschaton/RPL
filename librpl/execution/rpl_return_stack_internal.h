//
//  rpl_return_stack_internal.h
//  librpl
//
//  Created by Chris Hanson on 9/18/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_return_stack_internal__h__
#define __RPL__rpl_return_stack_internal__h__

#include "rpl_return_stack.h"


RPL_HEADER_BEGIN


struct rpl_return_stack {
    rpl_return_t RPL_NULLABLE * RPL_NONNULL _returns;
    rpl_integer_t _level;
    rpl_integer_t _depth;
};


RPL_HEADER_END


#endif /* __RPL__rpl_return_stack_internal__h__ */
