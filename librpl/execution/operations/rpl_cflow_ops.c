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
    {"$DO",        rpl_operation_type_command,rpl_cflow_op_$DO},
    {"$FORNEXT",   rpl_operation_type_command,rpl_cflow_op_$FORNEXT},
    {"$FORSTEP",   rpl_operation_type_command,rpl_cflow_op_$FORSTEP},
    {"$CASE",      rpl_operation_type_command,rpl_cflow_op_$CASE},
    {"$STARTNEXT", rpl_operation_type_command,rpl_cflow_op_$STARTNEXT},
    {"$STARTSTEP", rpl_operation_type_command,rpl_cflow_op_$STARTSTEP},
    {"$WHILE",     rpl_operation_type_command,rpl_cflow_op_$WHILE},
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
rpl_cflow_op_$DO(rpl_operation_t operation,
		 rpl_context_t context)
{
    // TODO: Implement $DO
    return false;
}

bool
rpl_cflow_op_$FORNEXT(rpl_operation_t operation,
		      rpl_context_t context)
{
    // TODO: Implement $FORNEXT
    return false;
}

bool
rpl_cflow_op_$FORSTEP(rpl_operation_t operation,
		      rpl_context_t context)
{
    // TODO: Implement $FORSTEP
    return false;
}

bool
rpl_cflow_op_$CASE(rpl_operation_t operation,
		   rpl_context_t context)
{
    // TODO: Implement $CASE
    return false;
}

bool
rpl_cflow_op_$STARTNEXT(rpl_operation_t operation,
			rpl_context_t context)
{
    // TODO: Implement $STARTNEXT
    return false;
}

bool
rpl_cflow_op_$STARTSTEP(rpl_operation_t operation,
			rpl_context_t context)
{
    // TODO: Implement $STARTSTEP
    return false;
}

bool
rpl_cflow_op_$WHILE(rpl_operation_t operation,
		    rpl_context_t context)
{
    // TODO: Implement $WHILE
    return false;
}


RPL_SOURCE_END
