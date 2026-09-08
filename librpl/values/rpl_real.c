//
//  rpl_real.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_real_internal.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN


rpl_value_t RPL_NULLABLE
rpl_real_new(rpl_real_t rep)
{
    rpl_value_t val = rpl_value_new(rpl_type_real);
    if (val) {
	val->_reps._real = rep;
    }
    return val;
}

void
rpl_real_free(rpl_value_t real)
{
    assert(real != NULL);
    assert(real->_type == rpl_type_real);

    /* Nothing to do. */
}

rpl_real_t
rpl_real_get_rep(rpl_value_t real)
{
    assert(real != NULL);
    assert(real->_type == rpl_type_real);

    return real->_reps._real;
}

rpl_unistring_t RPL_NULLABLE
rpl_real_copy_string(rpl_value_t real,
		     rpl_environment_t RPL_NULLABLE env)
{
    assert(real != NULL);
    assert(real->_type == rpl_type_real);

    return rpl_real_rep_copy_string(real->_reps._real, env);
}

rpl_unistring_t RPL_NULLABLE
rpl_real_rep_copy_string(rpl_real_t real_rep,
			 rpl_environment_t RPL_NULLABLE env)
{
    char buf[80] = { 0 };

    // TODO: Provide printing control in env

    snprintf(buf, 80, "%g", real_rep);

    return rpl_unistring_new_from_utf8(buf, strlen(buf));
}

rpl_unistring_t RPL_NULLABLE
rpl_real_copy_angle_string(rpl_value_t real,
			   rpl_environment_t RPL_NULLABLE env)
{
    assert(real != NULL);
    assert(real->_type == rpl_type_real);

    return rpl_real_rep_copy_angle_string(real->_reps._real, env);
}

rpl_unistring_t RPL_NULLABLE
rpl_real_rep_copy_angle_string(rpl_real_t real_rep,
			       rpl_environment_t RPL_NULLABLE env)
{
    const char * const cyl_angle_str = "∡";
    const size_t cyl_angle_str_len = strlen(cyl_angle_str);
    const char * const sph_angle_str = "∢";
    const size_t sph_angle_str_len = strlen(sph_angle_str);

    const rpl_angle_mode_t angle_mode
	= ((env == NULL)
	   ? rpl_angle_mode_degrees
	   : rpl_environment_get_angle_mode(env));

    double theta;
    switch (angle_mode) {
	case rpl_angle_mode_degrees:
	    theta = rpl_real_rep_r2d(real_rep);
	    break;
	case rpl_angle_mode_radians:
	    theta = real_rep;
	    break;
	case rpl_angle_mode_gradians:
	    theta = rpl_real_rep_r2g(real_rep);
	    break;
    }

    rpl_unistring_t angle_str = NULL;
    size_t angle_str_len = 0;

    /*
     The default coordinate system when none is supplied is the
     rectangular coordinaate system, and the user can also choose to
     use a rectangular coordinate system, but here we're explicitly
     describing an angle. Therefore default to cylindrical rather
     than rectangular coordinates.
     */
    rpl_coordinate_system_t coordinate_system
	= ((env == NULL)
	   ? rpl_coordinate_system_rectangular
	   : rpl_environment_get_coordinate_system(env));

    switch (coordinate_system) {
	case rpl_coordinate_system_rectangular:
	case rpl_coordinate_system_cylindrical: {
	    angle_str = rpl_unistring_new_from_utf8(cyl_angle_str,
						    cyl_angle_str_len);
	    angle_str_len = cyl_angle_str_len;
	} break;

	case rpl_coordinate_system_spherical: {
	    angle_str = rpl_unistring_new_from_utf8(sph_angle_str,
						    sph_angle_str_len);
	    angle_str_len = sph_angle_str_len;
	} break;
    }

    rpl_unistring_t theta_str = NULL;
    rpl_unistring_t buf = NULL;
    bool appended;

    theta_str = rpl_real_rep_copy_string(theta, env);
    if (theta_str == NULL) goto error;
    const size_t theta_str_len = rpl_unistring_get_length(theta_str);

    const size_t buf_size = angle_str_len + theta_str_len;
    buf = rpl_unistring_new(buf_size);
    if (buf == NULL) goto error;

    appended = rpl_unistring_append(buf, angle_str);
    if (appended == false) goto error;

    appended = rpl_unistring_append(buf, theta_str);
    if (appended == false) goto error;

    if (angle_str) rpl_unistring_release(angle_str);
    if (theta_str) rpl_unistring_release(theta_str);

    return buf;

error:
    if (angle_str) rpl_unistring_release(angle_str);
    if (theta_str) rpl_unistring_release(theta_str);
    if (buf) rpl_unistring_release(buf);
    return NULL;
}

rpl_value_t
rpl_real_pi(void)
{
    static rpl_value_t pi = NULL;
    if (pi == NULL) {
	pi = rpl_real_new(M_PI);
	assert(pi != NULL); /* very bad shape otherwise */
	rpl_value_immortalize(pi);
    }

    return pi;
}

rpl_value_t
rpl_real_pi_div_2(void)
{
    static rpl_value_t pi_div_2 = NULL;
    if (pi_div_2 == NULL) {
	pi_div_2 = rpl_real_new(M_PI_2);
	assert(pi_div_2 != NULL); /* very bad shape otherwise */
	rpl_value_immortalize(pi_div_2);
    }

    return pi_div_2;
}

rpl_real_t
rpl_real_rep_r2d(rpl_real_t theta)
{
    return theta * 180.0 * M_1_PI;
}

rpl_real_t
rpl_real_rep_r2g(rpl_real_t theta)
{
    return theta * 200.0 * M_1_PI;
}

rpl_real_t
rpl_real_rep_d2r(rpl_real_t theta_deg)
{
    return (theta_deg / 360.0) * (2.0 * M_PI);
}

rpl_real_t
rpl_real_rep_g2r(rpl_real_t theta_grad)
{
    return (theta_grad / 400.0) * (2.0 * M_PI);
}


RPL_SOURCE_END
