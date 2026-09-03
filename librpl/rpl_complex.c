//
//  rpl_complex.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_complex_internal.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN

rpl_value_t RPL_NULLABLE
rpl_complex_new(rpl_real_t a_rep, rpl_real_t b_rep)
{
    rpl_value_t complex = rpl_value_new(rpl_type_complex);
    if (complex) {
	complex->_reps._complex._a = a_rep;
	complex->_reps._complex._b = b_rep;
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

    a_str = rpl_real_rep_copy_string(complex_rep._a, env);
    if (a_str == NULL) goto error;
    const size_t a_str_len = strlen(a_str);

    b_str = rpl_real_rep_copy_string(complex_rep._b, env);
    if (b_str == NULL) goto error;
    const size_t b_str_len = strlen(b_str);

    /* (a, b) */
    const size_t buf_len = 1 + a_str_len + 2 + b_str_len + 1 + 1;
    buf = calloc(buf_len, sizeof(char));
    if (buf == NULL) goto error;

    strlcat(buf, "(", buf_len);
    strlcat(buf, a_str, buf_len);
    strlcat(buf, ", ", buf_len);
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




// TODO: Complex Number Operations


RPL_SOURCE_END
