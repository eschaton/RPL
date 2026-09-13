//
//  rpl_operations.c
//  librpl
//
//  Created by Chris Hanson on 9/12/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_operations.h"

#include "rpl_stack_ops.h"


RPL_SOURCE_BEGIN

bool
rpl_operations_configure(rpl_operation_table_t table)
{
    bool success = true;

    success = success && rpl_configure_stack_ops(table);
    /* success = success && rpl_configure_arith_ops(table); */
    /* success = success && rpl_configure_trig_ops(table); */
    /* success = success && rpl_configure_var_ops(table); */
    /* success = success && rpl_configure_integer_ops(table); */
    /* success = success && rpl_configure_array_ops(table); */
    /* success = success && rpl_configure_list_ops(table); */
    /* success = success && rpl_configure_program_ops(table); */

    return success;
}


RPL_SOURCE_END
