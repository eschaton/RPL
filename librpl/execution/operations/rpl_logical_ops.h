//
//  rpl_logical_ops.h
//  librpl
//
//  Created by Chris Hanson on 9/19/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_logical_ops__h__
#define __RPL__rpl_logical_ops__h__

#include "rpl_defines.h"

#include <stdbool.h>

#include "rpl_operation_table.h"


RPL_HEADER_BEGIN


/*! Configure all of the logical operations. */
bool
rpl_configure_logical_ops(rpl_operation_table_t table);


RPL_HEADER_END


#endif /* __RPL__rpl_logical_ops__h__ */
