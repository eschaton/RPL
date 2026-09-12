//
//  rpl_integer.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_integer_internal.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN


rpl_value_t RPL_NULLABLE
rpl_integer_new(rpl_integer_t rep)
RPL_RETURNS_RETAINED
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

rpl_unistring_t RPL_NULLABLE
rpl_integer_copy_string(rpl_value_t integer,
			rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED
{
    assert(integer != NULL);
    assert(integer->_type == rpl_type_integer);

    return rpl_integer_rep_copy_string(integer->_reps._integer, env);
}

rpl_unistring_t RPL_NULLABLE
rpl_integer_rep_copy_string(rpl_integer_t integer_rep,
			    rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED
{
    /* Use a local buffer of the longest possible string size. */

    char tmpbuf[68] = {0};

    /* "# valx" where "val" is value and "x" is one of hdob for base */

    const rpl_base_t base = (env
			     ? rpl_environment_get_base(env)
			     : rpl_base_decimal);
    switch (base) {
	case rpl_base_binary: {
	    tmpbuf[0] = '#';
	    tmpbuf[1]= ' ';

	    int digit = 2;
	    bool saw_one = false;
	    for (int i = 63; i >= 0; i--) {
		rpl_integer_t bit = (integer_rep >> i) & 1;
		if (bit == 0) {
		    if (saw_one) {
			tmpbuf[digit++] = '0';
		    }
		} else {
		    tmpbuf[digit++] = '1';
		    if (saw_one == false) saw_one = true;
		}
		if (i == 0) break;
	    }

	    tmpbuf[digit] = 'b';
	} break;

	case rpl_base_octal: {
	    snprintf(tmpbuf, 68, "# %lloo", integer_rep);
	} break;

	case rpl_base_decimal: {
	    snprintf(tmpbuf, 68, "# %llud", integer_rep);
	} break;

	case rpl_base_hexadecimal: {
	    snprintf(tmpbuf, 68, "# %llXh", integer_rep);
	} break;
    }

    const size_t tmpbuf_len = strlen(tmpbuf);

    return rpl_unistring_new_from_utf8(tmpbuf, tmpbuf_len);
}


RPL_SOURCE_END
