//
//  rpl_condition_ops_internal.h
//  librpl
//
//  Created by Chris Hanson on 10/17/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_condition_ops_internal__h__
#define __RPL__rpl_condition_ops_internal__h__

#include "rpl_condition_ops.h"

#include "rpl_operations_internal.h"



RPL_HEADER_BEGIN


/*
 The leading `_` below indicates the name of an internal function not
 intended to be invoked by users.
 */

RPL_OPERATION_DECLARE(condition, _CONDITION);
RPL_OPERATION_DECLARE(condition, _PROTECT);
RPL_OPERATION_DECLARE(condition, CONRCL);
RPL_OPERATION_DECLARE(condition, RESUME);
RPL_OPERATION_DECLARE(condition, RETRY);
RPL_OPERATION_DECLARE(condition, ABORT);


RPL_HEADER_END


#endif /* __RPL__rpl_condition_ops_internal__h__ */
