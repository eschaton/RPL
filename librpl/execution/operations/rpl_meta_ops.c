//
//  rpl_meta_ops.c
//  librpl
//
//  Created by Chris Hanson on 10/17/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_meta_ops_internal.h"

#include <assert.h>
#include <stdlib.h>
#include <sysexits.h>


RPL_SOURCE_BEGIN


rpl_op_definition_t rpl_meta_op_defs[] = {
    { "QUIT", rpl_operation_type_immediate, rpl_meta_op_QUIT },
    { NULL, 0, NULL },
};


bool
rpl_configure_meta_ops(rpl_operation_table_t table)
{
    assert(table != NULL);

    return rpl_operations_register_defs(table, rpl_meta_op_defs);
}


/* MARK: - Control Flow Operations */

bool
rpl_meta_op_QUIT(rpl_operation_t operation,
		 rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

    exit(EX_OK);

    /* Unreachable. */

    return true;
}


RPL_SOURCE_END
