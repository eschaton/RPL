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

// TODO: Complex Number Operations


RPL_SOURCE_END
