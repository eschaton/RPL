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

#include "rpl_operations_internal.h"


RPL_HEADER_BEGIN


RPL_OPERATION_DECLARE(stack, DUP);
RPL_OPERATION_DECLARE(stack, DROP);
RPL_OPERATION_DECLARE(stack, SWAP);


RPL_HEADER_END


#endif /* __RPL__rpl_stack_ops_internal__h__ */
