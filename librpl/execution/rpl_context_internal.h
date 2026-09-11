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
};


RPL_HEADER_END


#endif /* __RPL__rpl_context_internal__h__ */
