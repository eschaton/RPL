//
//  rpl_stack_internal.h
//  librpl
//
//  Created by Chris Hanson on 8/28/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_stack_internal__h__
#define __RPL__rpl_stack_internal__h__

#include "rpl_stack.h"


RPL_HEADER_BEGIN


struct rpl_stack {
    /*! Stack values, the actual stack contents. */
    rpl_value_t RPL_NULLABLE * RPL_NONNULL _values;

    /*! Stack pointer, index of the current top of stack. */
    rpl_integer_t _level;

    /*! Stack depth, total number of elements that can be pushed. */
    rpl_integer_t _depth;
};


RPL_HEADER_END


#endif /* __RPL__rpl_stack_internal__h__ */
