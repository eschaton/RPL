//
//  rpl_unit.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_unit_internal.h"

#include <assert.h>

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
	unit->_reps._unit._units = units;
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


RPL_SOURCE_END
