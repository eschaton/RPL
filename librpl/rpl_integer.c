//
//  rpl_integer.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_integer_internal.h"

#include <assert.h>

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


RPL_SOURCE_END
