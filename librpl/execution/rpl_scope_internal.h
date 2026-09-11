//
//  rpl_scope_internal.h
//  librpl
//
//  Created by Chris Hanson on 9/10/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_scope_internal__h__
#define __RPL__rpl_scope_internal__h__

#include "rpl_scope.h"
#include "rpl_adjbuffer.h"


RPL_HEADER_BEGIN


struct rpl_scope {
    rpl_scope_t RPL_NULLABLE _parent;
    rpl_adjbuffer_t _names;
    rpl_adjbuffer_t _values;
};


RPL_HEADER_END


#endif /* __RPL__rpl_scope_internal__h__ */
