//
//  rpl_name.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_name_internal.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN


/*
 TODO: Name interning mechanism

 Names should be interned and reference counted, so using the same names
 in a lot of different situations simultaneously is efficient.
 */


rpl_value_t RPL_NULLABLE
rpl_name_new(const char *rep, rpl_integer_t rep_len)
{
    assert(strlen(rep) == rep_len);

    rpl_value_t name = rpl_value_new(rpl_type_name);
    if (name) {
	name->_reps._name._chars = strdup(rep);
	if (name->_reps._name._chars == NULL) goto error;

	name->_reps._name._chars_len = rep_len;
    }
    return name;

error:
    rpl_value_release(name);
    return NULL;
}

void
rpl_name_free(rpl_value_t name)
{
    assert(name != NULL);
    assert(name->_type == rpl_type_name);

    free(name->_reps._name._chars);
}

const char *
rpl_name_get_rep(rpl_value_t name)
{
    assert(name != NULL);
    assert(name->_type == rpl_type_name);

    return name->_reps._name._chars;
}

rpl_integer_t
rpl_name_get_rep_len(rpl_value_t name)
{
    assert(name != NULL);
    assert(name->_type == rpl_type_name);

    return name->_reps._name._chars_len;
}


RPL_SOURCE_END
