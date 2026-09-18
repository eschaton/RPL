//
//  rpl_stack_ops_internal.h
//  librpl
//
//  Created by Chris Hanson on 9/12/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_stack_ops_internal__h__
#define __RPL__rpl_stack_ops_internal__h__

#include "rpl_stack_ops.h"


RPL_HEADER_BEGIN


bool
rpl_stack_op_DUP(rpl_operation_t operation,
		 rpl_context_t context);

bool
rpl_stack_op_DROP(rpl_operation_t operation,
		  rpl_context_t context);

bool
rpl_stack_op_SWAP(rpl_operation_t operation,
		  rpl_context_t context);


RPL_HEADER_END


#endif /* __RPL__rpl_stack_ops_internal__h__ */
