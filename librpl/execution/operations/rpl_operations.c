//
//  rpl_operations.c
//  librpl
//
//  Created by Chris Hanson on 9/12/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_operations_internal.h"

#include <assert.h>
#include <string.h>

#include "rpl_cflow_ops.h"
#include "rpl_logical_ops.h"
#include "rpl_stack_ops.h"
#include "rpl_variable_ops.h"


RPL_SOURCE_BEGIN

bool
rpl_operations_configure(rpl_operation_table_t table)
{
    bool success = true;

    /* success = success && rpl_configure_arith_ops(table); */
    /* success = success && rpl_configure_array_ops(table); */
    success = success && rpl_configure_cflow_ops(table);
    /* success = success && rpl_configure_integer_ops(table); */
    /* success = success && rpl_configure_list_ops(table); */
    success = success && rpl_configure_logical_ops(table);
    /* success = success && rpl_configure_program_ops(table); */
    success = success && rpl_configure_stack_ops(table);
    /* success = success && rpl_configure_trig_ops(table); */
    success = success && rpl_configure_variable_ops(table);

    return success;
}

bool
rpl_operations_register_defs(rpl_operation_table_t table,
			     rpl_op_definition_t *defs)
{
    assert(table != NULL);
    assert(defs != NULL);

    bool added;

    rpl_unistring_t name = NULL;
    rpl_operation_t op = NULL;

    for (rpl_op_definition_t *def = &defs[0];
	 (def->_name != NULL);
	 def++)
    {
	const size_t name_len = strlen(def->_name);
	assert(name_len != 0);

	name = rpl_unistring_new_from_utf8(def->_name, name_len);
	if (name == NULL) goto error;

	op = rpl_operation_new(name, def->_type, def->_impl, NULL);
	if (op == NULL) goto error;

	rpl_unistring_release(name); name = NULL;

	added = rpl_operation_table_set(table, op);
	if (added == false) goto error;
    }

    return true;

error:
    if (name) rpl_unistring_release(name);
    if (op) rpl_operation_free(op);
    return false;
}


RPL_SOURCE_END
