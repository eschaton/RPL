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


/*! Declare or define a control-flow operation. */
#define RPL_CFLOW_OP_DECLARE(name) \
    bool \
    rpl_cflow_op_ ## name (rpl_operation_t operation, \
			   rpl_context_t context)

/*! Declare or define a control-flow return operation. */
#define RPL_CFLOW_RET_DECLARE(name) \
    bool \
    rpl_cflow_op_ ## name ## _ret(rpl_return_t ret, \
				  rpl_context_t context)


/* MARK: - Control Flow Operations */

RPL_CFLOW_OP_DECLARE(IF);
RPL_CFLOW_OP_DECLARE(THEN);
RPL_CFLOW_OP_DECLARE(ELSE);

RPL_CFLOW_OP_DECLARE(DO);
RPL_CFLOW_OP_DECLARE(UNTIL);
RPL_CFLOW_OP_DECLARE(WHILE);
RPL_CFLOW_OP_DECLARE(REPEAT);

RPL_CFLOW_OP_DECLARE(END);

RPL_CFLOW_OP_DECLARE(FOR);
RPL_CFLOW_OP_DECLARE(START);
RPL_CFLOW_OP_DECLARE(NEXT);
RPL_CFLOW_OP_DECLARE(STEP);


/* MARK: - Control Flow Return Operations */

RPL_CFLOW_RET_DECLARE(IF);
RPL_CFLOW_RET_DECLARE(THEN);
RPL_CFLOW_RET_DECLARE(ELSE);

RPL_CFLOW_RET_DECLARE(DO);
RPL_CFLOW_RET_DECLARE(UNTIL);
RPL_CFLOW_RET_DECLARE(WHILE);
RPL_CFLOW_RET_DECLARE(REPEAT);

RPL_CFLOW_RET_DECLARE(FOR);
RPL_CFLOW_RET_DECLARE(START);


RPL_HEADER_END


#endif /* __RPL__rpl_cflow_ops_internal__h__ */
