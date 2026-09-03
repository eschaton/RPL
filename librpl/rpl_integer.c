//
//  rpl_integer.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_integer_internal.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN


rpl_value_t RPL_NULLABLE
rpl_integer_new(rpl_integer_t rep)
{
    rpl_value_t val = rpl_value_new(rpl_type_integer);
    if (val) {
	val->_reps._integer = rep;
    }
    return val;
}

void
rpl_integer_free(rpl_value_t integer)
{
    assert(integer != NULL);
    assert(integer->_type == rpl_type_integer);

    /* Nothing to do. */
}

rpl_integer_t
rpl_integer_get_rep(rpl_value_t integer)
{
    assert(integer != NULL);
    assert(integer->_type == rpl_type_integer);

    return integer->_reps._integer;
}

rpl_integer_t
rpl_integer_round_to_next(rpl_integer_t value, rpl_integer_t quantum)
{
    return value + (quantum - (value % quantum));
}

const char * RPL_NULLABLE
rpl_integer_copy_string(rpl_value_t integer)
{
    assert(integer != NULL);
    assert(integer->_type == rpl_type_integer);

    return rpl_integer_rep_copy_string(integer->_reps._integer);
}

const char * RPL_NULLABLE
rpl_integer_rep_copy_string(rpl_integer_t integer_rep)
{
    // TODO: Use base from a passed environment.

    /* At most 64 bits plus leading "# " and trailing base char. */

    char tmpbuf[68] = {0};

    /* "# valx" where "val" is value and "x" is one of hdob for base */

    snprintf(tmpbuf, 68, "# %llud", integer_rep);

    return strdup(tmpbuf);
}


RPL_SOURCE_END
