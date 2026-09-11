//
//  rpl_operation_internal.h
//  librpl
//
//  Created by Chris Hanson on 9/3/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_operation_internal__h__
#define __RPL__rpl_operation_internal__h__

#include "rpl_operation.h"


RPL_HEADER_BEGIN


struct rpl_operation {
    rpl_operation_type_t _type;
    const char *_name;
    size_t _name_len;
    rpl_operation_impl_t _impl;
    void * RPL_NULLABLE _refcon;
};


RPL_HEADER_END


#endif /* __RPL__rpl_operation_internal__h__*/
