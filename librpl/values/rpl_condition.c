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

#include "rpl_keyword.h"
#include "rpl_scope_internal.h"
#include "rpl_unistring_internal.h"
#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN


/*! Accumulate (key,value) pairs into a condition's payload. */
bool
rpl_condition_populate_payload(rpl_condition_t *rep, va_list args)
{
    rpl_value_t key = NULL;
    rpl_value_t value = NULL;

    do {
	key = va_arg(args, rpl_value_t);
	if (key) {
	    assert(rpl_value_get_type(key) == rpl_type_name);

	    value = va_arg(args, rpl_value_t);
	    if (value) {
		rpl_unistring_t name = rpl_name_get_rep(key);
		assert(name != NULL);

		bool did_set = rpl_scope_set_variable(rep->_payload,
						      name, value);
		if (did_set == false) goto error;
	    }
	}
    } while ((key != NULL) && (value != NULL));

    return true;

error:
    return false;
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

/*!
 Append a representation of the condition's payload to the string.

 The representation uses :tag:value pairs.
 */
bool
rpl_condition_append_payload(rpl_value_t condition,
			     rpl_unistring_t string,
			     rpl_environment_t env)
{
    assert(condition != NULL);
    assert(string != NULL);
    rpl_condition_t *rep = &condition->_reps._condition;

    const rpl_integer_t count = rpl_scope_get_count(rep->_payload);
    for (rpl_integer_t i = 0; i < count; i++) {
	rpl_unistring_t name = NULL;
	rpl_value_t value = NULL;
	bool appended = false;

	rpl_scope_get_pair_at_index(rep->_payload, i, &name, &value);

	rpl_unistring_t value_str = rpl_value_copy_string(value, env);
	if (value_str == NULL) goto error;

	appended = rpl_unistring_append_char(string, rpl_unichar_space);
	appended = appended && rpl_unistring_append(string, name);
	appended = appended
	    && rpl_unistring_append_char(string, rpl_unichar_space);
	appended = appended
	    && rpl_unistring_append(string, value_str);
	appended = appended
	    && rpl_unistring_append_char(string, rpl_unichar_linefeed);

	rpl_unistring_release(value_str);

	if (appended == false) goto error;
    }

    return true;

error:
    return false;
}

/*!
 Copy the string representation of a condition.

 The string representation of a condition is a list of `name object`
 pairs each on their own line between `CONDITION` and `END` lines:

     CONDITION
      NAME INSUFFICIENT_ARGUMENTS
      OPERATION '+'
      EXPECTED 2
     END

 A condition object may be interrogated using `CONDRCL`.
 */
rpl_unistring_t RPL_NULLABLE
rpl_condition_copy_string(rpl_value_t condition, rpl_environment_t env)
{
    assert(condition != NULL);
    rpl_condition_t *rep = &condition->_reps._condition;

    rpl_unistring_t string = NULL;
    bool appended = false;

    appended = appended
	&& rpl_unistring_append(string, rpl_keyword_CONDITION());
    appended = appended
	&& rpl_unistring_append_char(string, rpl_unichar_linefeed);

    appended = appended
	&& rpl_unistring_append_char(string, rpl_unichar_space);
    appended = appended
	&& rpl_unistring_append(string, rpl_keyword_NAME());
    appended = appended
	&& rpl_unistring_append_char(string, rpl_unichar_space);
    appended = appended
	&& rpl_unistring_append(string, rep->_name);
    appended = appended
	&& rpl_unistring_append_char(string, rpl_unichar_linefeed);

    appended = appended && rpl_condition_append_payload(condition,
							string, env);

    appended = appended && rpl_unistring_append(string,
						rpl_keyword_END());

    if (appended == false) goto error;

    return string;

error:
    if (string) rpl_unistring_release(string);
    return NULL;
}


/* MARK: - Standard Conditions */

rpl_value_t RPL_NULLABLE rpl_condition_memory_full_value = NULL;

rpl_value_t
rpl_condition_memory_full(void)
RPL_RETURNS_RETAINED
{
    return rpl_condition_memory_full_value;
}

rpl_value_t RPL_NULLABLE
rpl_condition_insufficient_arguments(rpl_value_t identifier,
				     rpl_integer_t expected,
				     rpl_integer_t actual)
RPL_RETURNS_RETAINED
{
    assert(identifier != NULL);
    assert(rpl_value_get_type(identifier) == rpl_type_identifier);

    rpl_value_t condition = NULL;
    rpl_value_t expectedv = NULL;
    rpl_value_t actualv = NULL;

    expectedv = rpl_integer_new(expected);
    if (expectedv == NULL) goto error;

    actualv = rpl_integer_new(actual);
    if (actualv == NULL) goto error;

    condition = rpl_condition_new(
		    rpl_keyword_INSUFFICIENT_ARGUMENTS(),
		    rpl_keyword_OPERATION(), identifier,
		    rpl_keyword_EXPECTED(), expectedv,
		    rpl_keyword_ACTUAL(), actualv,
		     NULL);
    if (condition == NULL) goto error;

    rpl_value_release(expectedv);
    rpl_value_release(actualv);

    return condition;

error:
    if (expectedv) rpl_value_release(expectedv);
    if (actualv) rpl_value_release(actualv);
    return NULL;
}

rpl_value_t RPL_NULLABLE
rpl_condition_invalid_type(rpl_value_t identifier,
			   rpl_type_t expected,
			   rpl_type_t actual)
RPL_RETURNS_RETAINED
{
    assert(identifier != NULL);
    assert(rpl_value_get_type(identifier) == rpl_type_identifier);

    rpl_value_t condition = NULL;
    rpl_value_t expectedv = NULL;
    rpl_value_t actualv = NULL;

    expectedv = rpl_value_get_type_name(expected);
    if (expectedv == NULL) goto error;

    actualv = rpl_value_get_type_name(actual);
    if (actualv == NULL) goto error;

    condition = rpl_condition_new(
		    rpl_keyword_INVALID_TYPE(),
		    rpl_keyword_OPERATION(), identifier,
		    rpl_keyword_EXPECTED(), expectedv,
		    rpl_keyword_ACTUAL(), actualv,
		     NULL);
    if (condition == NULL) goto error;

    rpl_value_release(expectedv);
    rpl_value_release(actualv);

    return condition;

error:
    if (expectedv) rpl_value_release(expectedv);
    if (actualv) rpl_value_release(actualv);
    return NULL;
}

bool
rpl_condition_initialize(void)
{
    /* Set up the static conditions. */

    rpl_condition_memory_full_value
	= rpl_condition_new(rpl_keyword_MEMORY_FULL(), NULL);
    if (rpl_condition_memory_full_value == NULL) goto error;
    rpl_value_immortalize(rpl_condition_memory_full_value);

    return true;

error:
    return false;
}


RPL_SOURCE_END

