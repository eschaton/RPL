//
//  rpl_arithmetic_ops.c
//  librpl
//
//  Created by Chris Hanson on 9/30/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//


#include "rpl_arithmetic_ops_internal.h"

#include <assert.h>

#include "rpl_name.h"
#include "rpl_operations_internal.h"


RPL_SOURCE_BEGIN


rpl_op_definition_t rpl_arithmetic_op_defs[] = {
    { "+", rpl_operation_type_function, rpl_arithmetic_op_$PLUS },
    { "-", rpl_operation_type_function, rpl_arithmetic_op_$MINUS },
    { "*", rpl_operation_type_function, rpl_arithmetic_op_$TIMES },
    { "/", rpl_operation_type_function, rpl_arithmetic_op_$DIVIDE },
    { NULL, 0, NULL },
};


bool
rpl_configure_arithmetic_ops(rpl_operation_table_t table)
{
    assert(table != NULL);

    return rpl_operations_register_defs(table, rpl_arithmetic_op_defs);
}


/* MARK: - Arithmetic Operations */

bool
rpl_arithmetic_op_$PLUS(rpl_operation_t operation,
			rpl_context_t context)
{
    // TODO: Implement $PLUS
    return false;
}

bool
rpl_arithmetic_op_$MINUS(rpl_operation_t operation,
			 rpl_context_t context)
{
    // TODO: Implement $MINUS
    return false;
}

bool
rpl_arithmetic_op_$TIMES(rpl_operation_t operation,
			 rpl_context_t context)
{
    // TODO: Implement $TIMES
    return false;
}

bool
rpl_arithmetic_op_$DIVIDE(rpl_operation_t operation,
			  rpl_context_t context)
{
    // TODO: Implement $DIVIDE
    return false;
}


RPL_SOURCE_END
