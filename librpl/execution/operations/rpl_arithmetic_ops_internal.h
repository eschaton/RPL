//
//  rpl_arithmetic_ops_internal.h
//  librpl
//
//  Created by Chris Hanson on 9/30/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_arithmetic_ops_internal__h__
#define __RPL__rpl_arithmetic_ops_internal__h__

#include "rpl_arithmetic_ops.h"

#include "rpl_operations_internal.h"


RPL_HEADER_BEGIN


/*
 The leading `$` below indicates the name of an internal function, since
 the actual symbols for these functions cannot be used in C identifiers.
 */

RPL_OPERATION_DECLARE(arithmetic, $PLUS);
RPL_OPERATION_DECLARE(arithmetic, $MINUS);
RPL_OPERATION_DECLARE(arithmetic, $TIMES);
RPL_OPERATION_DECLARE(arithmetic, $DIVIDE);


RPL_HEADER_END


#endif /* __RPL__rpl_arithmetic_ops_internal__h__ */
