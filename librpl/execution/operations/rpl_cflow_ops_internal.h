//
//  rpl_cflow_ops_internal.h
//  librpl
//
//  Created by Chris Hanson on 9/30/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_cflow_ops_internal__h__
#define __RPL__rpl_cflow_ops_internal__h__

#include "rpl_cflow_ops.h"

#include "rpl_operations_internal.h"



RPL_HEADER_BEGIN


/*
 The leading `_` below indicates the name of an internal function not
 intended to be invoked by users.
 */

RPL_OPERATION_DECLARE(cflow, IFT);
RPL_OPERATION_DECLARE(cflow, IFTE);
RPL_OPERATION_DECLARE(cflow, _DO);
RPL_OPERATION_DECLARE(cflow, _FORNEXT);
RPL_OPERATION_DECLARE(cflow, _FORSTEP);
RPL_OPERATION_DECLARE(cflow, _CASE);
RPL_OPERATION_DECLARE(cflow, _STARTNEXT);
RPL_OPERATION_DECLARE(cflow, _STARTSTEP);
RPL_OPERATION_DECLARE(cflow, _WHILE);


RPL_HEADER_END


#endif /* __RPL__rpl_cflow_ops_internal__h__ */
