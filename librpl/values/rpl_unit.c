//
//  rpl_unit.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_unit_internal.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN


rpl_value_t RPL_NULLABLE
rpl_unit_new(rpl_real_t scalar, rpl_value_t units)
{
    assert(units != NULL);
    assert(units->_type == rpl_type_name);

    rpl_value_t unit = rpl_value_new(rpl_type_unit);
    if (unit) {
	unit->_reps._unit._scalar = scalar;
	unit->_reps._unit._units = rpl_value_retain(units);
    }
    return unit;
}

void
rpl_unit_free(rpl_value_t unit)
{
    assert(unit != NULL);
    assert(unit->_type == rpl_type_unit);

    rpl_value_release(unit->_reps._unit._units);
}

rpl_real_t
rpl_unit_get_scalar_rep(rpl_value_t unit)
{
    assert(unit != NULL);
    assert(unit->_type == rpl_type_program);

    return unit->_reps._unit._scalar;
}

rpl_value_t
rpl_unit_get_units(rpl_value_t unit)
{
    assert(unit != NULL);
    assert(unit->_type == rpl_type_program);

    return unit->_reps._unit._units;
}

const char * RPL_NULLABLE
rpl_unit_copy_string(rpl_value_t unit,
		     rpl_environment_t RPL_NULLABLE env)
{
    assert(unit != NULL);
    assert(unit->_type == rpl_type_program);

    rpl_unit_t *rep = &unit->_reps._unit;

    const char *scalar_str = NULL;
    const char *units_str = NULL;
    char *buf = NULL;

    scalar_str = rpl_real_rep_copy_string(rep->_scalar, env);
    if (scalar_str == NULL) goto error;
    const size_t scalar_str_len = strlen(scalar_str);

    units_str = rpl_name_copy_string(rep->_units, env);
    if (units_str == NULL) goto error;
    const size_t units_str_len = strlen(units_str);

    /* real_units */
    
    const size_t buf_len = scalar_str_len + (units_str_len - 1) + 1;
    buf = calloc(buf_len, sizeof(char));
    if (buf == NULL) goto error;

    strlcpy(buf, scalar_str, buf_len);
    strlcat(buf, "_", buf_len);
    strlcat(buf, &units_str[1], buf_len); /* drops surrounding ' */

    free((void *)scalar_str);
    free((void *)units_str);

    return buf;

error:
    free((void *)scalar_str);
    free((void *)units_str);
    free(buf);
    return NULL;
}


RPL_SOURCE_END
