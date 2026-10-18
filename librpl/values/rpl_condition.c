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

#include "rpl_identifier.h"
#include "rpl_keyword.h"
#include "rpl_scope_internal.h"
#include "rpl_unistring_internal.h"
#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN


rpl_value_t RPL_NULLABLE
rpl_condition_new(void)
{
    rpl_value_t value = rpl_value_new(rpl_type_condition);
    if (value) {
	rpl_condition_t *rep = &value->_reps._condition;

	rep->_payload = rpl_scope_new(NULL);
	if (rep->_payload == NULL) goto error;
    }

    return value;

error:
    rpl_value_release(value);
    return NULL;
}

/*!
 Indicate whether the key is of the correct type.

 For convenience, a condition payload key may be either a name value or
 an identifier value. (The underlying implementation uses a unistring.)
 */
bool
rpl_condition_key_is_valid_type(rpl_value_t key)
{
    const rpl_type_t keyt = rpl_value_get_type(key);
    return ((keyt == rpl_type_name)
	    || (keyt == rpl_type_identifier));
}

/*!
 Get the name string for a condition key, which for convenience may be
 represented by either a name value or an identifier value.
 */
rpl_unistring_t RPL_NULLABLE
rpl_condition_key_name(rpl_value_t key)
{
    rpl_unistring_t name;
    const rpl_type_t keyt = rpl_value_get_type(key);
    if (keyt == rpl_type_name) {
	name = rpl_name_get_rep(key);
    } else if (keyt == rpl_type_identifier) {
	name = rpl_identifier_get_rep(key);
    } else {
	name = NULL;
    }

    return name;
}

/*! Accumulate (key,value) pairs into a condition's payload. */
bool
rpl_condition_populate_payload(rpl_condition_t *rep, va_list args)
{
    rpl_value_t key = NULL;
    rpl_value_t value = NULL;

    do {
	key = va_arg(args, rpl_value_t);
	if (key) {
	    assert(rpl_condition_key_is_valid_type(key));

	    value = va_arg(args, rpl_value_t);
	    if (value) {
		rpl_unistring_t name = rpl_condition_key_name(key);
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
rpl_condition_new_with_name(rpl_unistring_t name, ...)
{
    rpl_value_t value = rpl_condition_new();
    if (value) {
	rpl_condition_t *rep = &value->_reps._condition;

	bool did_set = rpl_condition_set_name(value, name);
	if (did_set == false) goto error;

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

bool
rpl_condition_set_name(rpl_value_t condition, rpl_unistring_t name)
{
    assert(condition != NULL);
    assert(name != NULL);
    rpl_condition_t *rep = &condition->_reps._condition;

    rpl_unistring_t temp = rpl_unistring_copy(name);
    if (temp) {
	if (rep->_name) rpl_unistring_release(rep->_name);
	rep->_name = temp;
	return true;
    } else {
	return false;
    }
}

rpl_unistring_t
rpl_condition_get_name(rpl_value_t condition)
{
    assert(condition != NULL);
    rpl_condition_t *rep = &condition->_reps._condition;

    return rep->_name;
}

bool
rpl_condition_set_payload_value(rpl_value_t condition,
				rpl_value_t key,
				rpl_value_t value)
{
    assert(condition != NULL);
    assert(key != NULL);
    assert(rpl_condition_key_is_valid_type(key));
    assert(value != NULL);
    rpl_condition_t *rep = &condition->_reps._condition;

    rpl_unistring_t name = rpl_condition_key_name(key);
    assert(name != NULL);

    return rpl_scope_set_variable(rep->_payload, name, value);
}

rpl_value_t
rpl_condition_get_payload_value(rpl_value_t condition, rpl_value_t key)
{
    assert(condition != NULL);
    assert(key != NULL);
    assert(rpl_condition_key_is_valid_type(key));
    rpl_condition_t *rep = &condition->_reps._condition;

    rpl_unistring_t name = rpl_condition_key_name(key);
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
    bool appended = true;

    string = rpl_unistring_new(128);

    appended = appended
	&& rpl_unistring_append(string, rpl_keyword_CONDITION());
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


/* MARK: - Signaling Conditions */

/*!
 The stack of signaled conditions.

 Newly-signaled conditions are appended.
 */
static rpl_adjbuffer_t rpl_condition_stack;

/*! Establish the condition stack. */
bool
rpl_condition_stack_create(void)
{
    return rpl_adjbuffer_init(&rpl_condition_stack, 8,
			      sizeof(rpl_value_t));
}

/*! Clear the condition stack. */
void
rpl_condition_stack_clear(void)
{
    (void) rpl_value_release_adjbuffer(&rpl_condition_stack);

    rpl_adjbuffer_remove_all_elements(&rpl_condition_stack);
}

/*! Push a condition on the stack. */
bool
rpl_condition_stack_append(rpl_value_t condition)
{
    rpl_value_retain(condition);

    bool appended = rpl_adjbuffer_append_element(&rpl_condition_stack,
						 &condition);

    rpl_value_not_leaked(condition);

    return appended;
}

static rpl_condition_handler_f rpl_condition_handler = NULL;
static void * RPL_NULLABLE rpl_condition_handler_context = NULL;

void
rpl_condition_handler_install(rpl_condition_handler_f handler,
			      void * RPL_NULLABLE context)
{
    assert(handler != NULL);

    rpl_condition_handler = handler;
    rpl_condition_handler_context = context;
}

/*! Signal a condition. */
void
rpl_condition_signal(rpl_value_t condition)
{
    bool appended = rpl_condition_stack_append(condition);
    assert(appended);

    if (rpl_condition_handler) {
	size_t idx = rpl_adjbuffer_get_count(&rpl_condition_stack) - 1;

	(*rpl_condition_handler)(condition, idx,
				 rpl_condition_handler_context);
    }
}

rpl_value_t RPL_NULLABLE
rpl_condition_latest_signaled(void)
RPL_RETURNS_RETAINED
{
    rpl_value_t result = NULL;
    rpl_value_t *cp = rpl_adjbuffer_last_element(&rpl_condition_stack);
    if (cp != NULL) {
	result = *cp;
    }
    return result;
}

size_t
rpl_condition_stack_get_count(void)
{
    return rpl_adjbuffer_get_count(&rpl_condition_stack);
}

/*! Get the condition at the given level of the condition stack. */
rpl_value_t
rpl_condition_stack_get(size_t level)
{
    assert(level < rpl_condition_stack_get_count());

    rpl_value_t *cp = rpl_adjbuffer_get(&rpl_condition_stack, level);
    assert(cp != NULL);

    return *cp;
}

bool
rpl_condition_stack_reset(void)
{
    rpl_condition_stack_clear();

    return true;
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

    condition = rpl_condition_new_with_name(
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

    condition = rpl_condition_new_with_name(
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
    /* Set up the condition stack. */
    bool stack_initialized = rpl_condition_stack_create();
    if (stack_initialized == false) goto error;

    /* Set up the static conditions. */

    rpl_condition_memory_full_value
	= rpl_condition_new_with_name(rpl_keyword_MEMORY_FULL(), NULL);
    if (rpl_condition_memory_full_value == NULL) goto error;
    rpl_value_immortalize(rpl_condition_memory_full_value);

    return true;

error:
    return false;
}


RPL_SOURCE_END

