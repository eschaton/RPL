//
//  rpl_complex.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_complex_internal.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN

rpl_value_t RPL_NULLABLE
rpl_complex_new(rpl_real_t x_rep, rpl_real_t y_rep)
RPL_RETURNS_RETAINED
{
    rpl_value_t complex = rpl_value_new(rpl_type_complex);
    if (complex) {
	complex->_reps._complex._x = x_rep;
	complex->_reps._complex._y = y_rep;
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

rpl_unistring_t RPL_NULLABLE
rpl_complex_copy_string(rpl_value_t complex,
			rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED
{
    assert(complex != NULL);
    assert(complex->_type == rpl_type_complex);

    return rpl_complex_rep_copy_string(complex->_reps._complex, env);
}

rpl_unistring_t RPL_NULLABLE
rpl_complex_rep_copy_string(rpl_complex_t complex_rep,
			    rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED
{
    rpl_unistring_t a_str = NULL;
    rpl_unistring_t b_str = NULL;
    rpl_unistring_t buf = NULL;
    bool appended;

    const rpl_coordinate_system_t coordinate_system
	= ((env == NULL)
	   ? rpl_coordinate_system_rectangular
	   : rpl_environment_get_coordinate_system(env));

    switch (coordinate_system) {
	case rpl_coordinate_system_rectangular: {
	    /* x and y are printable as-is */

	    a_str = rpl_real_rep_copy_string(complex_rep._x, env);
	    if (a_str == NULL) goto error;

	    b_str = rpl_real_rep_copy_string(complex_rep._y, env);
	    if (b_str == NULL) goto error;
	} break;

	case rpl_coordinate_system_cylindrical:
	case rpl_coordinate_system_spherical:{
	    /* x and y must be converted to r and theta */

	    rpl_real_t r = rpl_complex_rep_get_modulus(complex_rep);
	    rpl_real_t th = rpl_complex_rep_get_argument(complex_rep);

	    a_str = rpl_real_rep_copy_string(r, env);
	    if (a_str == NULL) goto error;

	    b_str = rpl_real_rep_copy_angle_string(th, env);
	    if (b_str == NULL) goto error;
	} break;
    }

    const size_t a_str_len = rpl_unistring_get_length(a_str);
    const size_t b_str_len = rpl_unistring_get_length(b_str);

    /* (x,y) or (r,∡θ) */
    const size_t buf_len = 1 + a_str_len + 1 + b_str_len + 1;
    buf = rpl_unistring_new(buf_len);
    if (buf == NULL) goto error;

    appended = rpl_unistring_append_char(buf,
					 rpl_unichar_parenthesis_open);
    if (appended == false) goto error;

    appended = rpl_unistring_append(buf, a_str);
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(buf, rpl_unichar_comma);
    if (appended == false) goto error;

    appended = rpl_unistring_append(buf, a_str);
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(buf,
					 rpl_unichar_parenthesis_close);
    if (appended == false) goto error;

    rpl_unistring_release(a_str);
    rpl_unistring_release(b_str);

    return buf;

error:
    if (a_str) rpl_unistring_release(a_str);
    if (b_str) rpl_unistring_release(b_str);
    if (buf) rpl_unistring_release(buf);
    return NULL;
}

rpl_real_t
rpl_complex_rep_get_modulus(rpl_complex_t rectangular)
{
    return hypot(rectangular._x, rectangular._y);
}

rpl_real_t
rpl_complex_rep_get_argument(rpl_complex_t rectangular)
{
    return atan2(rectangular._y, rectangular._x);
}


// TODO: Complex Number Operations


RPL_SOURCE_END
