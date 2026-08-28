//
//  rpl_complex.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_complex_internal.h"

#include <assert.h>

#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN


rpl_value_t RPL_NULLABLE
rpl_complex_rectangular_new(rpl_real_t x_rep, rpl_real_t y_rep)
{
    rpl_value_t complex = rpl_value_new(rpl_type_complex);
    if (complex) {
	complex->_reps._complex._is_polar = false;
	complex->_reps._complex._value._rectangular._x = x_rep;
	complex->_reps._complex._value._rectangular._y = y_rep;
    }
    return complex;
}

rpl_value_t RPL_NULLABLE
rpl_complex_polar_new(rpl_real_t r_rep, rpl_real_t theta_rep)
{
    rpl_value_t complex = rpl_value_new(rpl_type_complex);
    if (complex) {
	complex->_reps._complex._is_polar = true;
	complex->_reps._complex._value._polar._r = r_rep;
	complex->_reps._complex._value._polar._theta = theta_rep;
    }
    return complex;
}

void
rpl_complex_free(rpl_value_t complex)
{
    assert(complex != NULL);
    assert(complex->_type == rpl_type_complex);

    /* Nothing to do. */
}

// TODO: Complex Number Operations


RPL_SOURCE_END
