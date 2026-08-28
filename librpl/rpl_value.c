//
//  rpl_value.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_value_internal.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


RPL_SOURCE_BEGIN


rpl_value_t RPL_NULLABLE
rpl_value_new(rpl_type_t type)
{
    rpl_value_t val = calloc(1, sizeof(struct rpl_value));
    if (val) {
	val->_type = type;
    }
    return val;
}

void
rpl_value_free(rpl_value_t val)
{
    assert(val != NULL);

    switch (val->_type) {
	case rpl_type_integer: rpl_integer_free(val); break;
	case rpl_type_real:    rpl_real_free(val);    break;
	case rpl_type_complex: rpl_complex_free(val); break;
	case rpl_type_array:   rpl_array_free(val);   break;
	case rpl_type_name:    rpl_name_free(val);    break;
	case rpl_type_program: rpl_program_free(val); break;
	case rpl_type_string:  rpl_string_free(val);  break;
	case rpl_type_list:    rpl_list_free(val);    break;
	case rpl_type_tagged:  rpl_tagged_free(val);  break;
	case rpl_type_unit:    rpl_unit_free(val);    break;
    }

    free(val);
}

void
rpl_value_free_array(rpl_value_t RPL_NONNULL * RPL_NONNULL vals,
		     rpl_integer_t vals_count)
{
    assert(vals != NULL);
    assert(vals_count > 0);

    for (rpl_integer_t i = 0; i < vals_count; i++) {
	rpl_value_t val = vals[i];
	rpl_value_free(val);
    }
}

rpl_type_t
rpl_value_get_type(rpl_value_t val)
{
    return val->_type;
}


RPL_SOURCE_END
