//
//  rpl_stack_ops.h
//  librpl
//
//  Created by Chris Hanson on 9/11/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_stack_ops__h__
#define __RPL__rpl_stack_ops__h__

#include "rpl_defines.h"

#include <stdbool.h>

#include "rpl_operation_table.h"


RPL_HEADER_BEGIN


/*! Configure all of the stack operations. */
bool
rpl_configure_stack_ops(rpl_operation_table_t table);


RPL_HEADER_END


#endif /* __RPL__rpl_stack_ops__h__ */
