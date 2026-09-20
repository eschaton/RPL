//
//  rpl_logical_ops_internal.h
//  librpl
//
//  Created by Chris Hanson on 9/19/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_logical_ops_internal__h__
#define __RPL__rpl_logical_ops_internal__h__

#include "rpl_logical_ops.h"


RPL_HEADER_BEGIN


/*! Declare or define a logical operation. */
#define RPL_LOGICAL_OP_DECLARE(name) \
    bool \
    rpl_logical_op_ ## name (rpl_operation_t operation, \
			     rpl_context_t context)

RPL_LOGICAL_OP_DECLARE(LT);
RPL_LOGICAL_OP_DECLARE(LTE);
RPL_LOGICAL_OP_DECLARE(GT);
RPL_LOGICAL_OP_DECLARE(GTE);
RPL_LOGICAL_OP_DECLARE(EQUAL);
RPL_LOGICAL_OP_DECLARE(NOTEQUAL);


RPL_HEADER_END


#endif /* __RPL__rpl_logical_ops_internal__h__ */
