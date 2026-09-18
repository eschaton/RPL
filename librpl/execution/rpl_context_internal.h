//
//  rpl_context_internal.h
//  librpl
//
//  Created by Chris Hanson on 9/3/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_context_internal__h__
#define __RPL__rpl_context_internal__h__

#include "rpl_context.h"


RPL_HEADER_BEGIN


struct rpl_context {
    rpl_environment_t _environment;
    rpl_stack_t _stack;
    rpl_return_stack_t _return_stack;
    rpl_scope_t _constant;
    rpl_scope_t _global;
    rpl_scope_t _local;
};


RPL_HEADER_END


#endif /* __RPL__rpl_context_internal__h__ */
