//
//  rpl_operation_table_internal.h
//  librpl
//
//  Created by Chris Hanson on 9/11/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_operation_table_internal__h__
#define __RPL__rpl_operation_table_internal__h__

#include "rpl_operation_table.h"

#include "rpl_adjbuffer.h"


RPL_HEADER_BEGIN


struct rpl_operation_table {
    rpl_adjbuffer_t _names;
    rpl_adjbuffer_t _ops;
};


RPL_HEADER_END


#endif /* __RPL__rpl_operation_table_internal__h__ */
