//
//  rpl_program.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_program_internal.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "rpl_keyword.h"
#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN


rpl_value_t RPL_NULLABLE
rpl_program_new_with_type(rpl_program_type_t type)
{
    rpl_value_t val = rpl_value_new(rpl_type_program);
    if (val) {
	rpl_program_t *rep = &val->_reps._program;
	rep->_type = type;
	bool inited = rpl_adjbuffer_init(&rep->_values, 8,
					 sizeof(rpl_value_t));
	if (inited == false) goto error;
    }
    return val;

error:
    rpl_value_release(val);
    return NULL;
}

rpl_value_t RPL_NULLABLE
rpl_program_new(void)
{
    return rpl_program_new_with_type(rpl_program_type_generic);
}

void
rpl_program_free(rpl_value_t program)
{
    assert(program != NULL);
    assert(program->_type == rpl_type_program);
    rpl_program_t *rep = &program->_reps._program;

    (void) rpl_value_release_adjbuffer(&rep->_values);

    rpl_adjbuffer_deinit(&rep->_values);
}

rpl_integer_t
rpl_program_get_count(rpl_value_t program)
{
    assert(program != NULL);
    assert(program->_type == rpl_type_program);
    rpl_program_t *rep = &program->_reps._program;

    return rpl_adjbuffer_get_count(&rep->_values);
}

rpl_value_t
rpl_program_get_value(rpl_value_t program, rpl_integer_t idx)
{
    assert(program != NULL);
    assert(program->_type == rpl_type_program);
    assert(idx < rpl_program_get_count(program));
    rpl_program_t *rep = &program->_reps._program;

    rpl_value_t *element = rpl_adjbuffer_get(&rep->_values, idx);
    assert(element != NULL);

    return *element;
}

bool
rpl_program_append(rpl_value_t program, rpl_value_t value)
{
    assert(program != NULL);
    assert(program->_type == rpl_type_program);
    assert(value != NULL);
    rpl_program_t *rep = &program->_reps._program;

    rpl_value_retain(value);

    return rpl_adjbuffer_append_element(&rep->_values, &value);
}

rpl_unistring_t RPL_NULLABLE
rpl_program_generic_copy_string(rpl_value_t program,
				rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED
{
    assert(program != NULL);
    assert(program->_type == rpl_type_program);
    rpl_program_t *rep = &program->_reps._program;
    assert(rep->_type == rpl_program_type_generic);

    bool appended = false;
    rpl_unistring_t vs = NULL;
    rpl_unistring_t buffer = rpl_unistring_new(16);
    if (buffer == NULL) goto error;

    appended = rpl_unistring_append_char(buffer,
					 rpl_unichar_chevron_open);
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(buffer, rpl_unichar_space);
    if (appended == false) goto error;

    const rpl_integer_t count = rpl_program_get_count(program);
    for (rpl_integer_t i = 0; i < count; i++) {
	rpl_value_t val = rpl_program_get_value(program, i);
	assert(val != NULL);

	vs = rpl_value_copy_string(val, env);
	if (vs == NULL) goto error;

	appended = rpl_unistring_append(buffer, vs);
	if (appended == false) goto error;

	rpl_unistring_release(vs); vs = NULL;

	appended = rpl_unistring_append_char(buffer, rpl_unichar_space);
	if (appended == false) goto error;
    }

    appended = rpl_unistring_append_char(buffer,
					 rpl_unichar_chevron_close);
    if (appended == false) goto error;

    return buffer;

error:
    if (vs) rpl_unistring_release(vs);
    if (buffer) rpl_unistring_release(buffer);
    return NULL;
}

rpl_unistring_t RPL_NULLABLE
rpl_program_intermediate_copy_string(rpl_value_t program,
				     rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED
{
    assert(program != NULL);
    assert(program->_type == rpl_type_program);
    rpl_program_t *rep = &program->_reps._program;
    assert(rep->_type == rpl_program_type_intermediate);

    bool appended = false;
    rpl_unistring_t vs = NULL;
    rpl_unistring_t buffer = rpl_unistring_new(16);
    if (buffer == NULL) goto error;

    const rpl_integer_t count = rpl_program_get_count(program);
    for (rpl_integer_t i = 0; i < count; i++) {
	rpl_value_t val = rpl_program_get_value(program, i);
	assert(val != NULL);

	vs = rpl_value_copy_string(val, env);
	if (vs == NULL) goto error;

	appended = rpl_unistring_append(buffer, vs);
	if (appended == false) goto error;

	rpl_unistring_release(vs); vs = NULL;

	if (i != (count - 1)) {
	    appended = rpl_unistring_append_char(buffer,
						 rpl_unichar_space);
	    if (appended == false) goto error;
	}
    }

    return buffer;

error:
    if (vs) rpl_unistring_release(vs);
    if (buffer) rpl_unistring_release(buffer);
    return NULL;
}

rpl_unistring_t RPL_NULLABLE
rpl_program_DO_copy_string(rpl_value_t program,
			   rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED
{
    // TODO: Copy string for DO
    return NULL;
}

/*!
 Copy a string representing an `IF ... THEN ... [ELSE ...] END` program.

 The two possibilities be distinguished by the number of values, either
 three or four, depending on whether there is an `ELSE ...` clause. The
 first value will be an intermediate program representing the condition,
 the second the `THEN ...` clause, the third the `ELSE ...` clause if
 one is present, and the final value will be the identifier `IFT` if
 there is no `ELSE ...` clause or `IFTE` if there is one.
 */
rpl_unistring_t RPL_NULLABLE
rpl_program_IF_copy_string(rpl_value_t program,
			   rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED
{
    assert(program != NULL);
    assert(program->_type == rpl_type_program);
    rpl_program_t *rep = &program->_reps._program;
    assert(rep->_type == rpl_program_type_IF);

    bool has_else = (rpl_program_get_count(program) == 4);
    bool appended = false;
    rpl_unistring_t cond_str = NULL;
    rpl_unistring_t then_str = NULL;
    rpl_unistring_t else_str = NULL;
    rpl_unistring_t buffer = rpl_unistring_new(16);
    if (buffer == NULL) goto error;

    appended = rpl_unistring_append(buffer, rpl_keyword_IF());
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(buffer, rpl_unichar_space);
    if (appended == false) goto error;

    rpl_value_t cond_prog = rpl_program_get_value(program, 0);
    assert(cond_prog != NULL);

    cond_str = rpl_program_intermediate_copy_string(cond_prog, env);
    if (cond_str == NULL) goto error;

    appended = rpl_unistring_append(buffer, cond_str);
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(buffer, rpl_unichar_space);
    if (appended == false) goto error;

    appended = rpl_unistring_append(buffer, rpl_keyword_THEN());
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(buffer, rpl_unichar_space);
    if (appended == false) goto error;

    rpl_value_t then_prog = rpl_program_get_value(program, 1);
    assert(then_prog != NULL);

    then_str = rpl_program_intermediate_copy_string(then_prog, env);
    if (then_str == NULL) goto error;

    appended = rpl_unistring_append(buffer, then_str);
    if (appended == false) goto error;

    if (has_else) {
	appended = rpl_unistring_append_char(buffer, rpl_unichar_space);
	if (appended == false) goto error;

	appended = rpl_unistring_append(buffer, rpl_keyword_ELSE());
	if (appended == false) goto error;

	appended = rpl_unistring_append_char(buffer, rpl_unichar_space);
	if (appended == false) goto error;

	rpl_value_t else_prog = rpl_program_get_value(program, 2);
	assert(else_prog != NULL);

	else_str = rpl_program_intermediate_copy_string(else_prog, env);
	if (else_str == NULL) goto error;

	appended = rpl_unistring_append(buffer, else_str);
	if (appended == false) goto error;
    }

    appended = rpl_unistring_append_char(buffer, rpl_unichar_space);
    if (appended == false) goto error;

    appended = rpl_unistring_append(buffer, rpl_keyword_END());
    if (appended == false) goto error;

    rpl_unistring_release(cond_str);
    rpl_unistring_release(then_str);
    if (else_str) rpl_unistring_release(else_str);

    return buffer;

error:
    if (cond_str) rpl_unistring_release(cond_str);
    if (then_str) rpl_unistring_release(then_str);
    if (else_str) rpl_unistring_release(else_str);
    if (buffer) rpl_unistring_release(buffer);
    return NULL;
}

rpl_unistring_t RPL_NULLABLE
rpl_program_FOR_copy_string(rpl_value_t program,
			    rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED
{
    // TODO: Copy string for FOR
    return NULL;
}

rpl_unistring_t RPL_NULLABLE
rpl_program_CASE_copy_string(rpl_value_t program,
			     rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED
{
    // TODO: Copy string for CASE
    return NULL;
}

rpl_unistring_t RPL_NULLABLE
rpl_program_START_copy_string(rpl_value_t program,
			      rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED
{
    // TODO: Copy string for START
    return NULL;
}

rpl_unistring_t RPL_NULLABLE
rpl_program_WHILE_copy_string(rpl_value_t program,
			      rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED
{
    // TODO: Copy string for WHILE
    return NULL;
}

rpl_unistring_t RPL_NULLABLE
rpl_program_copy_string(rpl_value_t program,
			rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED
{
    assert(program != NULL);
    assert(program->_type == rpl_type_program);
    rpl_program_t *rep = &program->_reps._program;

    switch (rep->_type) {
	case rpl_program_type_generic:
	    return rpl_program_generic_copy_string(program, env);

	case rpl_program_type_intermediate:
	    return rpl_program_intermediate_copy_string(program, env);

	case rpl_program_type_DO:
	    return rpl_program_DO_copy_string(program, env);

	case rpl_program_type_IF:
	    return rpl_program_IF_copy_string(program, env);

	case rpl_program_type_FOR:
	    return rpl_program_FOR_copy_string(program, env);

	case rpl_program_type_CASE:
	    return rpl_program_CASE_copy_string(program, env);

	case rpl_program_type_START:
	    return rpl_program_START_copy_string(program, env);

	case rpl_program_type_WHILE:
	    return rpl_program_WHILE_copy_string(program, env);
    }
}

rpl_program_type_t
rpl_program_get_type(rpl_value_t program)
{
    assert(program != NULL);
    rpl_program_t *rep = &program->_reps._program;

    return rep->_type;
}


RPL_SOURCE_END
