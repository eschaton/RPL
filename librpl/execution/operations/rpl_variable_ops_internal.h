//
//  rpl_variable_ops_internal.h
//  RPL
//
//  Created by Chris Hanson on 9/17/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_variable_ops_internal__h__
#define __RPL__rpl_variable_ops_internal__h__

#include "rpl_variable_ops.h"


RPL_HEADER_BEGIN


bool
rpl_variable_op_RCL(rpl_operation_t operation,
		    rpl_context_t context);

bool
rpl_variable_op_STO(rpl_operation_t operation,
		    rpl_context_t context);


RPL_HEADER_END


#endif /* __RPL__rpl_variable_ops_internal__h__ */
