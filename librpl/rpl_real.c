//
//  rpl_real.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_real_internal.h"

#include <assert.h>
#include <stdlib.h>
#include <math.h>
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

const char * RPL_NULLABLE
rpl_real_copy_string(rpl_value_t real,
		     rpl_environment_t RPL_NULLABLE env)
{
    assert(real != NULL);
    assert(real->_type == rpl_type_real);

    return rpl_real_rep_copy_string(real->_reps._real, env);
}

const char * RPL_NULLABLE
rpl_real_rep_copy_string(rpl_real_t real_rep,
			 rpl_environment_t RPL_NULLABLE env)
{
    // TODO: rpl_real_rep_copy_string
    return NULL;
}

const char * RPL_NULLABLE
rpl_real_rep_copy_angle_string(rpl_real_t real_rep,
			       rpl_environment_t RPL_NULLABLE env)
{
    const char * const angle_str = "∡";
    const size_t angle_str_len = strlen(angle_str);

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

    const char *theta_str = NULL;
    char *buf = NULL;

    theta_str = rpl_real_rep_copy_string(theta, env);
    if (theta_str == NULL) goto error;
    const size_t theta_str_len = strlen(theta_str);

    const size_t buf_size = angle_str_len + theta_str_len + 1;
    buf = calloc(buf_size, sizeof(char));
    if (buf == NULL) goto error;

    strlcpy(buf, angle_str, buf_size);
    strlcpy(buf, theta_str, buf_size);

    free((void *)theta_str);

    return buf;

error:
    free((void *)theta_str);
    free(buf);

    return NULL;
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
