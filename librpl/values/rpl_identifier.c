//
//  rpl_identifier.c
//  librpl
//
//  Created by Chris Hanson on 9/21/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_identifier_internal.h"

#include <assert.h>
#include <stdlib.h>

#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN


rpl_value_t RPL_NULLABLE
rpl_identifier_new(rpl_unistring_t str)
{
    assert(str != NULL);

    rpl_value_t val = rpl_value_new(rpl_type_identifier);
    if (val) {
	rpl_identifier_t *rep = &val->_reps._identifier;
	rep->_str = rpl_unistring_copy(str);
	if (rep->_str == NULL) goto error;
    }
    return val;

error:
    rpl_value_release(val);
    return NULL;
}

void
rpl_identifier_free(rpl_value_t val)
{
    assert(val != NULL);
    rpl_identifier_t *rep = &val->_reps._identifier;

    if (rep->_str) rpl_unistring_release(rep->_str);
}

rpl_unistring_t
rpl_identifier_get_rep(rpl_value_t val)
{
    assert(val != NULL);
    rpl_identifier_t *rep = &val->_reps._identifier;

    return rep->_str;
}

rpl_unistring_t RPL_NULLABLE
rpl_identifier_copy_string(rpl_value_t val, rpl_environment_t env)
RPL_RETURNS_RETAINED
{
    assert(val != NULL);
    rpl_identifier_t *rep = &val->_reps._identifier;

    return rpl_unistring_copy(rep->_str);
}


RPL_SOURCE_END
