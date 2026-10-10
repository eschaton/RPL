//
//  rpl_condition.c
//  librpl
//
//  Created by Chris Hanson on 10/9/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_condition_internal.h"

#include <assert.h>
#include <stdlib.h>

#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN


bool
rpl_condition_populate_payload(rpl_condition_t *rep, va_list args)
{
    // TODO: Implement rpl_condition_populate_payload
    return true;
}


rpl_value_t RPL_NULLABLE
rpl_condition_new(rpl_unistring_t name, ...)
{
    rpl_value_t value = rpl_value_new(rpl_type_condition);
    if (value) {
	rpl_condition_t *rep = &value->_reps._condition;

	rep->_name = rpl_unistring_copy(name);
	if (rep->_name == NULL) goto error;

	rep->_payload = rpl_scope_new(NULL);
	if (rep->_payload == NULL) goto error;

	va_list ap;
	va_start(ap, name);
	bool populated = rpl_condition_populate_payload(rep, ap);
	va_end(ap);

	if (populated == false) goto error;
    }

    return value;

error:
    rpl_value_release(value);
    return NULL;
}

void
rpl_condition_free(rpl_value_t condition)
{
    assert(condition != NULL);
    rpl_condition_t *rep = &condition->_reps._condition;

    if (rep->_name) rpl_unistring_release(rep->_name);
    if (rep->_payload) rpl_scope_free(rep->_payload);
}

rpl_unistring_t
rpl_condition_get_name(rpl_value_t condition)
{
    assert(condition != NULL);
    rpl_condition_t *rep = &condition->_reps._condition;

    return rep->_name;
}

rpl_value_t
rpl_condition_get_payload_value(rpl_value_t condition, rpl_value_t key)
{
    assert(condition != NULL);
    assert(key != NULL);
    assert(rpl_value_get_type(key) == rpl_type_name);
    rpl_condition_t *rep = &condition->_reps._condition;

    rpl_unistring_t name = rpl_name_get_rep(key);
    assert(name != NULL);

    return rpl_scope_get_variable(rep->_payload, name, false);
}

rpl_unistring_t RPL_NULLABLE
rpl_condition_copy_string(rpl_value_t condition, rpl_environment_t env)
{
    assert(condition != NULL);
    rpl_condition_t *rep = &condition->_reps._condition;

    // TODO: Implement rpl_condition_copy_string

    return rpl_unistring_copy(rep->_name);
}


RPL_SOURCE_END

