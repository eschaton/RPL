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

const char * RPL_NULLABLE
rpl_complex_copy_string(rpl_value_t complex,
			rpl_environment_t RPL_NULLABLE env)
{
    assert(complex != NULL);
    assert(complex->_type == rpl_type_complex);

    return rpl_complex_rep_copy_string(complex->_reps._complex, env);
}

const char * RPL_NULLABLE
rpl_complex_rep_copy_string(rpl_complex_t complex_rep,
			    rpl_environment_t RPL_NULLABLE env)
{
    const char *a_str = NULL;
    const char *b_str = NULL;
    char *buf = NULL;

    const rpl_complex_mode_t complex_mode
	= ((env == NULL)
	   ? rpl_complex_mode_rectangular
	   : rpl_environment_get_complex_mode(env));

    switch (complex_mode) {
	case rpl_complex_mode_rectangular: {
	    /* x and y are printable as-is */

	    a_str = rpl_real_rep_copy_string(complex_rep._x, env);
	    if (a_str == NULL) goto error;

	    b_str = rpl_real_rep_copy_string(complex_rep._y, env);
	    if (b_str == NULL) goto error;
	} break;

	case rpl_complex_mode_cylindrical:
	case rpl_complex_mode_spherical:{
	    /* x and y must be converted to r and theta */

	    rpl_real_t r = rpl_complex_rep_get_modulus(complex_rep);
	    rpl_real_t th = rpl_complex_rep_get_argument(complex_rep);

	    a_str = rpl_real_rep_copy_string(r, env);
	    if (a_str == NULL) goto error;

	    b_str = rpl_real_rep_copy_angle_string(th, env);
	    if (b_str == NULL) goto error;
	} break;
    }

    const size_t a_str_len = strlen(a_str);
    const size_t b_str_len = strlen(b_str);

    /* (x,y) or (r,∡θ) */
    const size_t buf_len = 1 + a_str_len + 1 + b_str_len + 1 + 1;
    buf = calloc(buf_len, sizeof(char));
    if (buf == NULL) goto error;

    strlcat(buf, "(", buf_len);
    strlcat(buf, a_str, buf_len);
    strlcat(buf, ",", buf_len);
    strlcat(buf, b_str, buf_len);
    strlcat(buf, ")", buf_len);

    free((void *)a_str);
    free((void *)b_str);

    return buf;

error:
    free((void *)a_str);
    free((void *)b_str);
    free(buf);
    return NULL;
}

rpl_real_t
rpl_complex_rep_get_modulus(rpl_complex_t rectangular)
{
    return sqrt((rectangular._x * rectangular._x)
		+ (rectangular._y * rectangular._y));
}

rpl_real_t
rpl_complex_rep_get_argument(rpl_complex_t rectangular)
{
    return atan2(rectangular._y, rectangular._x);
}


// TODO: Complex Number Operations


RPL_SOURCE_END
