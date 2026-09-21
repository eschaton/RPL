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
	val->_refs = 1;
    }
    return val;
}

void
rpl_value_free(rpl_value_t val)
{
    assert(val != NULL);
    assert(val->_refs == 0);

    switch (val->_type) {
	case rpl_type_integer:    rpl_integer_free(val);    break;
	case rpl_type_real:       rpl_real_free(val);       break;
	case rpl_type_complex:    rpl_complex_free(val);    break;
	case rpl_type_array:      rpl_array_free(val);      break;
	case rpl_type_name:       rpl_name_free(val);       break;
	case rpl_type_program:    rpl_program_free(val);    break;
	case rpl_type_string:     rpl_string_free(val);     break;
	case rpl_type_list:       rpl_list_free(val);       break;
	case rpl_type_tagged:     rpl_tagged_free(val);     break;
	case rpl_type_unit:       rpl_unit_free(val);       break;
	case rpl_type_identifier: rpl_identifier_free(val); break;
    }

    free(val);
}

void
rpl_value_release_array(rpl_value_t RPL_NULLABLE * RPL_NONNULL vals,
			rpl_integer_t vals_count)
{
    assert(vals != NULL);
    assert(vals_count > 0);

    for (rpl_integer_t i = 0; i < vals_count; i++) {
	rpl_value_t val = vals[i];
	/* Note that we allow NULL values here for array. */
	if (val != NULL) rpl_value_release(val);
    }
}

rpl_type_t
rpl_value_get_type(rpl_value_t val)
{
    return val->_type;
}

rpl_value_t
rpl_value_retain(rpl_value_t val)
{
    assert(val != NULL);
    assert(val->_refs >= 0);

    if (val->_refs != RPL_INTEGER_MAX) {
	val->_refs += 1;

	/* Assert if this object became permanent. */
	assert(val->_refs != RPL_INTEGER_MAX);
    }
    
    return val;
}

void
rpl_value_release(rpl_value_t val RPL_RELEASES_ARGUMENT)
{
    assert(val != NULL);
    assert(val->_refs > 0);

    if (val->_refs != RPL_INTEGER_MAX) {
	val->_refs -= 1;

	if (val->_refs == 0) {
	    rpl_value_free(val);
	}
    }
}

void
rpl_value_immortalize(rpl_value_t val)
{
    assert(val != NULL);
    assert(val->_refs > 0);
    assert(val->_refs != RPL_INTEGER_MAX);
    
    val->_refs = RPL_INTEGER_MAX;
}

void
rpl_value_not_leaked(rpl_value_t val RPL_RELEASES_ARGUMENT)
{
    assert(val != NULL);

    /*
     Nothing else to do: This function exists purely to supply its
     argument annotation to the static analyzer. (This is mainly for
     the situation where an "RPL_RETAINS_ARGUMENT" annotation would be
     useful, but such a thing doesn't exist.)
     */
}

rpl_value_t RPL_NULLABLE
rpl_value_copy(rpl_value_t val)
RPL_RETURNS_RETAINED
{
    assert(val != NULL);

    rpl_value_t result = NULL;

    switch (val->_type) {
	    /* Immutable types are just retained. */
	case rpl_type_integer:
	case rpl_type_real:
	case rpl_type_complex:
	case rpl_type_name:
	case rpl_type_program:
	case rpl_type_tagged:
	case rpl_type_unit:
	case rpl_type_identifier:
	    result = rpl_value_retain(val);
	    break;

	    /* Mutable types have copy functions. */
	case rpl_type_array:
	    result = rpl_array_copy(val);
	    break;

	case rpl_type_string:
	    result = rpl_string_copy(val);
	    break;

	case rpl_type_list:
	    result = rpl_list_copy(val);
	    break;
    }

    return result;
}

rpl_unistring_t RPL_NULLABLE
rpl_value_copy_string(rpl_value_t val,
		      rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED
{
    assert(val != NULL);

    switch (val->_type) {
	case rpl_type_integer: return rpl_integer_copy_string(val, env);
	case rpl_type_real:    return rpl_real_copy_string(val, env);
	case rpl_type_complex: return rpl_complex_copy_string(val, env);
	case rpl_type_array:   return rpl_array_copy_string(val, env);
	case rpl_type_name:    return rpl_name_copy_string(val, env);
	case rpl_type_program: return rpl_program_copy_string(val, env);
	case rpl_type_string:  return rpl_string_copy_string(val, env);
	case rpl_type_list:    return rpl_list_copy_string(val, env);
	case rpl_type_tagged:  return rpl_tagged_copy_string(val, env);
	case rpl_type_unit:    return rpl_unit_copy_string(val, env);
	case rpl_type_identifier:
	    return rpl_identifier_copy_string(val, env);
    }

    return NULL;
}


RPL_SOURCE_END
