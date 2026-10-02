//
//  rpl_cflow_ops.c
//  librpl
//
//  Created by Chris Hanson on 9/30/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//


#include "rpl_cflow_ops_internal.h"

#include <assert.h>

#include "rpl_name.h"


RPL_SOURCE_BEGIN


rpl_op_definition_t rpl_cflow_op_defs[] = {
    {"IFT",        rpl_operation_type_command,rpl_cflow_op_IFT},
    {"IFTE",       rpl_operation_type_command,rpl_cflow_op_IFTE},
    {"_DO",        rpl_operation_type_command,rpl_cflow_op__DO},
    {"_FORNEXT",   rpl_operation_type_command,rpl_cflow_op__FORNEXT},
    {"_FORSTEP",   rpl_operation_type_command,rpl_cflow_op__FORSTEP},
    {"_CASE",      rpl_operation_type_command,rpl_cflow_op__CASE},
    {"_STARTNEXT", rpl_operation_type_command,rpl_cflow_op__STARTNEXT},
    {"_STARTSTEP", rpl_operation_type_command,rpl_cflow_op__STARTSTEP},
    {"_WHILE",     rpl_operation_type_command,rpl_cflow_op__WHILE},
    {NULL, 0, NULL },
};


bool
rpl_configure_cflow_ops(rpl_operation_table_t table)
{
    assert(table != NULL);

    return rpl_operations_register_defs(table, rpl_cflow_op_defs);
}


/* MARK: - Control Flow Operations */

bool
rpl_cflow_op_IFT(rpl_operation_t operation,
		 rpl_context_t context)
{
    // TODO: Implement IFT
    return false;
}

bool
rpl_cflow_op_IFTE(rpl_operation_t operation,
		  rpl_context_t context)
{
    // TODO: Implement IFTE
    return false;
}

bool
rpl_cflow_op__DO(rpl_operation_t operation,
		 rpl_context_t context)
{
    // TODO: Implement iDO
    return false;
}

bool
rpl_cflow_op__FORNEXT(rpl_operation_t operation,
		      rpl_context_t context)
{
    // TODO: Implement iFORNEXT
    return false;
}

bool
rpl_cflow_op__FORSTEP(rpl_operation_t operation,
		      rpl_context_t context)
{
    // TODO: Implement iFORSTEP
    return false;
}

bool
rpl_cflow_op__CASE(rpl_operation_t operation,
		   rpl_context_t context)
{
    // TODO: Implement iCASE
    return false;
}

bool
rpl_cflow_op__STARTNEXT(rpl_operation_t operation,
			rpl_context_t context)
{
    // TODO: Implement iSTARTNEXT
    return false;
}

bool
rpl_cflow_op__STARTSTEP(rpl_operation_t operation,
			rpl_context_t context)
{
    // TODO: Implement iSTARTSTEP
    return false;
}

bool
rpl_cflow_op__WHILE(rpl_operation_t operation,
		    rpl_context_t context)
{
    // TODO: Implement iWHILE
    return false;
}


RPL_SOURCE_END
