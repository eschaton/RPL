//
//  rpl_return.c
//  librpl
//
//  Created by Chris Hanson on 9/18/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_return_internal.h"

#include <assert.h>
#include <stdlib.h>


RPL_SOURCE_BEGIN


rpl_return_t RPL_NULLABLE
rpl_return_new(rpl_return_operation_t operation,
	       rpl_integer_t level)
{
    assert(operation != NULL);

    rpl_return_t ret = calloc(1, sizeof(struct rpl_return));
    if (ret) {
	bool initialized
	    = rpl_adjbuffer_init(&ret->_entries, 4,
				 sizeof(struct rpl_return_entry));
	if (initialized == false) goto error;

	/* Add the first entry. */
	rpl_return_entry_t first = { operation, level };
	bool appended
	    = rpl_adjbuffer_append_element(&ret->_entries, &first);
	if (appended == false) goto error;

	/* Mark the return as open. */
	ret->_is_closed = false;
    }
    return ret;

error:
    rpl_return_free(ret);
    return NULL;
}

void
rpl_return_free(rpl_return_t ret)
{
    assert(ret != NULL);
    /* Do not check that the return is closed. */

    rpl_adjbuffer_deinit(&ret->_entries);

    free(ret);
}

bool
rpl_return_is_open(rpl_return_t ret)
{
    assert(ret != NULL);

    return (ret->_is_closed == false);
}

rpl_integer_t
rpl_return_get_count(rpl_return_t ret)
{
    assert(ret != NULL);

    return rpl_adjbuffer_get_count(&ret->_entries);
}

rpl_return_operation_t
rpl_return_get_entry(rpl_return_t ret, rpl_integer_t idx,
		     rpl_integer_t *level)
{
    assert(ret != NULL);
    assert(idx < rpl_return_get_count(ret));
    assert(level != NULL);

    rpl_return_entry_t *entry = rpl_adjbuffer_get(&ret->_entries, idx);
    assert(entry != NULL);

    *level = entry->_level;

    return entry->_operation;
}

bool
rpl_return_append(rpl_return_t ret,
		  rpl_return_operation_t operation,
		  rpl_integer_t level)
{
    assert(ret != NULL);
    assert(operation != NULL);
    assert(ret->_is_closed == false);

    rpl_return_entry_t entry = { operation, level };

    return rpl_adjbuffer_append_element(&ret->_entries, &entry);
}

void
rpl_return_close(rpl_return_t ret)
{
    assert(ret != NULL);
    assert(ret->_is_closed == false);

    ret->_is_closed = true;
}

bool
rpl_return_invoke(rpl_return_t ret, rpl_context_t context)
{
    assert(ret != NULL);
    assert(ret->_is_closed);
    assert(context != NULL);

    rpl_return_entry_t *first = rpl_adjbuffer_get(&ret->_entries, 0);
    assert(first != NULL);

    return (*first->_operation)(ret, context);
}


RPL_SOURCE_END
