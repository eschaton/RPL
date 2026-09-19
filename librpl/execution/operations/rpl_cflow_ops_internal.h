//
//  rpl_cflow_ops_internal.h
//  librpl
//
//  Created by Chris Hanson on 9/18/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_cflow_ops_internal__h__
#define __RPL__rpl_cflow_ops_internal__h__

#include "rpl_cflow_ops.h"


RPL_HEADER_BEGIN


bool
rpl_cflow_op_IF(rpl_operation_t operation,
		rpl_context_t context);

bool
rpl_cflow_op_THEN(rpl_operation_t operation,
		  rpl_context_t context);

bool
rpl_cflow_op_ELSE(rpl_operation_t operation,
		  rpl_context_t context);

bool
rpl_cflow_op_END(rpl_operation_t operation,
		 rpl_context_t context);


RPL_HEADER_END


#endif /* __RPL__rpl_cflow_ops_internal__h__ */
