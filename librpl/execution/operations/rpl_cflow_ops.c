//
//  rpl_cflow_ops.c
//  RPL
//
//  Created by Chris Hanson on 9/18/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_cflow_ops_internal.h"

#include <assert.h>

#include "rpl_operations_internal.h"


RPL_SOURCE_BEGIN


rpl_op_definition_t rpl_cflow_op_defs[] = {
    { "IF",	rpl_operation_type_immediate, rpl_cflow_op_IF },
    { "THEN",	rpl_operation_type_immediate, rpl_cflow_op_THEN },
    { "ELSE",	rpl_operation_type_immediate, rpl_cflow_op_ELSE },
    { "END",	rpl_operation_type_immediate, rpl_cflow_op_END },
    { NULL, 0, NULL },
};


bool
rpl_configure_cflow_ops(rpl_operation_table_t table)
{
    assert(table != NULL);

    return rpl_operations_register_defs(table, rpl_cflow_op_defs);
}


/* MARK: - Control Flow Operations */

bool
rpl_cflow_op_IF(rpl_operation_t operation, rpl_context_t context)
{
    return false;//xxx
}

bool
rpl_cflow_op_THEN(rpl_operation_t operation, rpl_context_t context)
{
    return false;//xxx
}

bool
rpl_cflow_op_ELSE(rpl_operation_t operation, rpl_context_t context)
{
    return false;//xxx
}

bool
rpl_cflow_op_END(rpl_operation_t operation, rpl_context_t context)
{
    return false;//xxx
}


RPL_SOURCE_END
