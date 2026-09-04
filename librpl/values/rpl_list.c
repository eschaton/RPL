//
//  rpl_list.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_list_internal.h"

#include <assert.h>

#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN


rpl_value_t RPL_NULLABLE
rpl_list_new(void)
{
    return rpl_value_new(rpl_type_list);
}

rpl_value_t RPL_NULLABLE
rpl_list_new_with_pair(rpl_value_t RPL_NULLABLE car,
		       rpl_value_t RPL_NULLABLE cdr)
{
    rpl_value_t list = rpl_list_new();
    if (list) {
	list->_reps._list._car = car;
	list->_reps._list._cdr = cdr;
    }
    return list;
}

rpl_value_t RPL_NULLABLE
rpl_list_new_with_array(rpl_value_t RPL_NONNULL * RPL_NONNULL elements,
			rpl_integer_t elements_count)
{
    assert(elements != NULL);
    assert(elements_count > 0);

    rpl_value_t list = rpl_list_new();
    if (list) {
	list->_reps._list._car = elements[0];
	rpl_value_t current = list;
	for (rpl_integer_t i = 1; i < elements_count; i++) {
	    rpl_value_t element = elements[i];
	    assert(element != NULL);

	    rpl_value_t pair = rpl_list_new_with_pair(element, NULL);
	    if (pair == NULL) goto error;

	    current->_reps._list._cdr = pair;
	    current = pair;
	}
    }
    return list;

error:
    rpl_value_release(list);
    return list;
}

void
rpl_list_free(rpl_value_t list)
{
    assert(list != NULL);
    assert(list->_type == rpl_type_list);

    // TODO: Free list
}

rpl_value_t RPL_NULLABLE
rpl_list_copy(rpl_value_t list)
{
    // TODO: rpl_list_copy

    return NULL;
}

rpl_value_t
rpl_list_car(rpl_value_t list)
{
    assert(list != NULL);
    assert(list->_type == rpl_type_list);

    return list->_reps._list._car;
}

rpl_value_t
rpl_list_cdr(rpl_value_t val)
{
    assert(val != NULL);
    assert(val->_type == rpl_type_list);

    return val->_reps._list._cdr;
}

void
rpl_list_append(rpl_value_t list, rpl_value_t list2)
{
    assert(list != NULL);
    assert(list->_type == rpl_type_list);
    assert(list2 != NULL);
    assert(list2->_type == rpl_type_list);

    /* Follow list's cdr to a node that has a NULL cdr. */

    rpl_value_t next = list;
    rpl_value_t tail = NULL;
    do {
	assert(next->_type == rpl_type_list);
	if (next->_reps._list._cdr == NULL) {
	    tail = next;
	}
	next = next->_reps._list._cdr;
    } while (tail != NULL);

    /* Set the tail's cdr to val. */

    tail->_reps._list._cdr = list2;
}

const char * RPL_NULLABLE
rpl_list_copy_string(rpl_value_t list,
		     rpl_environment_t RPL_NULLABLE env)
{
    // TODO: rpl_list_copy_string
    return NULL;
}


RPL_SOURCE_END
