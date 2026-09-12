//
//  rpl_operation_table.c
//  librpl
//
//  Created by Chris Hanson on 9/11/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_operation_table_internal.h"

#include <assert.h>
#include <stdlib.h>


RPL_SOURCE_BEGIN


rpl_operation_table_t RPL_NULLABLE
rpl_operation_table_new(void)
{
    rpl_operation_table_t table
	= calloc(1, sizeof(struct rpl_operation_table));
    if (table) {
	bool initialized;

	initialized = rpl_adjbuffer_init(&table->_names, 256,
					 sizeof(rpl_unistring_t));
	if (initialized == false) goto error;

	initialized = rpl_adjbuffer_init(&table->_ops, 256,
					 sizeof(rpl_operation_t));
	if (initialized == false) goto error;
    }

    return table;

error:
    rpl_operation_table_free(table);
    return NULL;
}

void
rpl_operation_table_free(rpl_operation_table_t table)
{
    assert(table != NULL);

    rpl_adjbuffer_deinit(&table->_names);
    rpl_adjbuffer_deinit(&table->_ops);

    free(table);
}

bool
rpl_operation_table_name_is_equal(rpl_adjbuffer_t *buffer,
				  void *element0, void *element1,
				  void * RPL_NULLABLE refcon)
{
    assert(buffer != NULL);
    assert(element0 != NULL);
    assert(element1 != NULL);

    rpl_unistring_t str0 = element0;
    rpl_unistring_t str1 = element1;

    return rpl_unistring_compare(str0, str1) == 0;
}

ssize_t
rpl_operation_table_find(rpl_operation_table_t table,
			 rpl_unistring_t name)
{
    assert(table != NULL);
    assert(name != NULL);

    return rpl_adjbuffer_find(&table->_names, name,
			      rpl_operation_table_name_is_equal, NULL);
}

rpl_operation_t RPL_NULLABLE
rpl_operation_table_get(rpl_operation_table_t table,
			rpl_unistring_t name)
{
    assert(table != NULL);
    assert(name != NULL);

    rpl_operation_t op = NULL;

    ssize_t idx = rpl_operation_table_find(table, name);
    if (idx != -1) {
	op = rpl_adjbuffer_get(&table->_ops, idx);
    }

    return op;
}

bool
rpl_operation_table_set(rpl_operation_table_t table,
			rpl_operation_t op)
{
    assert(table != NULL);
    assert(op != NULL);

    bool success = false;

    rpl_unistring_t name = rpl_operation_get_name(op);
    assert(name != NULL);

    /* Replacement is not allowed. */
    assert(rpl_operation_table_find(table, name) == -1);

    bool appended = rpl_adjbuffer_append_element(&table->_names, &name);
    if (appended) {
	appended = rpl_adjbuffer_append_element(&table->_ops, &op);
	if (appended) {
	    success = true;
	} else {
	    const size_t name_idx
	    = rpl_adjbuffer_get_count(&table->_names) - 1;
	    rpl_adjbuffer_remove_element(&table->_names, name_idx);
	}
    }

    return success;
}


RPL_SOURCE_END
