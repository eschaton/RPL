//
//  rpl_operations.h
//  librpl
//
//  Created by Chris Hanson on 9/12/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_operations__h__
#define __RPL__rpl_operations__h__

#include "rpl_defines.h"

#include <stdbool.h>

#include "rpl_operation_table.h"


RPL_HEADER_BEGIN


/*! Configure all of the standard operations. */
RPL_EXPORT
bool
rpl_operations_configure(rpl_operation_table_t table);


RPL_HEADER_END


#endif /* __RPL__rpl_operations__h__ */
