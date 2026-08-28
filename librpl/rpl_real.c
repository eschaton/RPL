//
//  rpl_real.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_real_internal.h"

#include <assert.h>

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


RPL_SOURCE_END
