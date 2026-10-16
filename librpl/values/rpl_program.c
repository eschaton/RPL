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

 Note that this also attempts to format the program "nicely," at least
 insofar as the `IF` construct is concerned: The `IF`, `THEN`, `ELSE`,
 and `END` are all on separate lines and start at column 0, the other
 clauses are on their own lines and start at column 2, and all end with
 a linefeed.
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

    appended = rpl_unistring_append_char(buffer, rpl_unichar_linefeed);
    if (appended == false) goto error;

    rpl_value_t cond_prog = rpl_program_get_value(program, 0);
    assert(cond_prog != NULL);

    appended = rpl_unistring_append_char(buffer, rpl_unichar_space);
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(buffer, rpl_unichar_space);
    if (appended == false) goto error;

    cond_str = rpl_program_intermediate_copy_string(cond_prog, env);
    if (cond_str == NULL) goto error;

    appended = rpl_unistring_append(buffer, cond_str);
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(buffer, rpl_unichar_linefeed);
    if (appended == false) goto error;

    appended = rpl_unistring_append(buffer, rpl_keyword_THEN());
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(buffer, rpl_unichar_linefeed);
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(buffer, rpl_unichar_space);
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(buffer, rpl_unichar_space);
    if (appended == false) goto error;

    rpl_value_t then_prog = rpl_program_get_value(program, 1);
    assert(then_prog != NULL);

    then_str = rpl_program_intermediate_copy_string(then_prog, env);
    if (then_str == NULL) goto error;

    appended = rpl_unistring_append(buffer, then_str);
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(buffer, rpl_unichar_linefeed);
    if (appended == false) goto error;

    if (has_else) {
	appended = rpl_unistring_append(buffer, rpl_keyword_ELSE());
	if (appended == false) goto error;

	appended = rpl_unistring_append_char(buffer,
					     rpl_unichar_linefeed);
	if (appended == false) goto error;

	appended = rpl_unistring_append_char(buffer, rpl_unichar_space);
	if (appended == false) goto error;

	appended = rpl_unistring_append_char(buffer, rpl_unichar_space);
	if (appended == false) goto error;

	rpl_value_t else_prog = rpl_program_get_value(program, 2);
	assert(else_prog != NULL);

	else_str = rpl_program_intermediate_copy_string(else_prog, env);
	if (else_str == NULL) goto error;

	appended = rpl_unistring_append(buffer, else_str);
	if (appended == false) goto error;

	appended = rpl_unistring_append_char(buffer,
					     rpl_unichar_linefeed);
	if (appended == false) goto error;
    }

    appended = rpl_unistring_append(buffer, rpl_keyword_END());
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(buffer, rpl_unichar_linefeed);
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

/*!
 Copy a string representing a `CONDITION` program.

 The construct that looks like

     CONDITION name
       { key value }
     END

 is represented by a program of the form

     « name { key value } key-value-count _CONDITION »

 so that information needs to be extracted and put back into a string.
 The condition name is always at index `0` and the number of key/value
 pairs is always at index `count - 2`.
 */
rpl_unistring_t RPL_NULLABLE
rpl_program_CONDITION_copy_string(rpl_value_t program,
				  rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED
{
    assert(program != NULL);
    assert(program->_type == rpl_type_program);
    rpl_program_t *rep = &program->_reps._program;
    assert(rep->_type == rpl_program_type_CONDITION);

    rpl_unistring_t string = NULL;
    rpl_unistring_t vs = NULL;
    bool appended = false;

    const rpl_integer_t count = rpl_program_get_count(program);
    assert(count >= 3);

    rpl_value_t namev = rpl_program_get_value(program, 0);
    assert(namev != NULL);
    assert(rpl_value_get_type(namev) == rpl_type_identifier);

    const rpl_value_t kvcv = rpl_program_get_value(program, count - 2);
    assert(kvcv != NULL);
    assert(rpl_value_get_type(kvcv) == rpl_type_integer);
    const rpl_integer_t kvc = rpl_integer_get_rep(kvcv);

    assert(count == (1 + (kvc * 2) + 2));

    string = rpl_unistring_new(80);
    if (string == NULL) goto error;

    appended = rpl_unistring_append(string, rpl_keyword_CONDITION());
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(string, rpl_unichar_space);
    if (appended == false) goto error;

    rpl_unistring_t names = rpl_identifier_get_rep(namev);
    assert(names != NULL);

    appended = rpl_unistring_append(string, names);
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(string, rpl_unichar_linefeed);
    if (appended == false) goto error;

    for (rpl_integer_t i = 0; i < kvc; i++) {
	const rpl_integer_t kidx = 1 + (2 * i);
	const rpl_integer_t vidx = 2 + (2 * i);

	rpl_value_t k = rpl_program_get_value(program, kidx);
	assert(k != NULL);
	assert(rpl_value_get_type(k) == rpl_type_identifier);

	rpl_value_t v = rpl_program_get_value(program, vidx);
	assert(v != NULL);

	rpl_unistring_t ks = rpl_identifier_get_rep(k);
	assert(ks != NULL);

	vs = rpl_value_copy_string(v, env);
	if (vs == NULL) goto error;

	appended = rpl_unistring_append_char(string, rpl_unichar_space);
	if (appended == false) goto error;

	appended = rpl_unistring_append_char(string, rpl_unichar_space);
	if (appended == false) goto error;

	appended = rpl_unistring_append(string, ks);
	if (appended == false) goto error;

	appended = rpl_unistring_append_char(string, rpl_unichar_space);
	if (appended == false) goto error;

	appended = rpl_unistring_append(string, vs);
	if (appended == false) goto error;

	appended = rpl_unistring_append_char(string,
					     rpl_unichar_linefeed);
	if (appended == false) goto error;

	rpl_unistring_release(vs); vs = NULL;
    }

    appended = rpl_unistring_append(string, rpl_keyword_END());
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(string, rpl_unichar_linefeed);
    if (appended == false) goto error;

    return string;

error:
    if (string) rpl_unistring_release(string);
    if (vs) rpl_unistring_release(vs);
    return NULL;
}

/*!
 Copy a string representing a `PROTECT` program.

 The construct that looks like

     PROTECT
       body
       { HANDLE cond BY handler END }
       [ generic-handler ]
     END

 is represented by a program of the form

     «
       « body »
       { condition « handler » }
       condition-handler-pair-count
       « generic-handler »
       _PROTECT
     »

 so that information needs to be extracted and put back into a string.
 The body is always at index `0`, the number of condition/handler pairs
 is always at index `count - 3`, and the generic handler is always at
 index `count - 2`.
 */
rpl_unistring_t RPL_NULLABLE
rpl_program_PROTECT_copy_string(rpl_value_t program,
			      rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED
{
    assert(program != NULL);
    assert(program->_type == rpl_type_program);
    rpl_program_t *rep = &program->_reps._program;
    assert(rep->_type == rpl_program_type_PROTECT);

    rpl_unistring_t string = NULL;
    bool appended = false;

    const rpl_integer_t count = rpl_program_get_count(program);
    assert(count >= 4);

    string = rpl_unistring_new(80);
    if (string == NULL) goto error;

    appended = rpl_unistring_append(string, rpl_keyword_PROTECT());
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(string, rpl_unichar_linefeed);
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(string, rpl_unichar_space);
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(string, rpl_unichar_space);
    if (appended == false) goto error;

    rpl_value_t b = rpl_program_get_value(program, 0);
    assert(b != NULL);
    assert(rpl_value_get_type(b) == rpl_type_program);

    rpl_unistring_t bs = rpl_program_intermediate_copy_string(b, env);
    if (bs == NULL) goto error;

    appended = rpl_unistring_append(string, bs);
    rpl_unistring_release(bs);
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(string, rpl_unichar_linefeed);
    if (appended == false) goto error;

    const rpl_value_t chcv = rpl_program_get_value(program, count - 3);
    assert(chcv != NULL);
    assert(rpl_value_get_type(chcv) == rpl_type_integer);
    const rpl_integer_t chc = rpl_integer_get_rep(chcv);

    for (rpl_integer_t i = 0; i < chc; i++) {
	const rpl_integer_t cidx = 1 + (2 * i);
	const rpl_integer_t hidx = 2 + (2 * i);

	appended = rpl_unistring_append_char(string, rpl_unichar_space);
	if (appended == false) goto error;

	appended = rpl_unistring_append_char(string, rpl_unichar_space);
	if (appended == false) goto error;

	appended = rpl_unistring_append(string, rpl_keyword_HANDLE());
	if (appended == false) goto error;

	appended = rpl_unistring_append_char(string, rpl_unichar_space);
	if (appended == false) goto error;

	rpl_value_t cv = rpl_program_get_value(program, cidx);
	assert(cv != NULL);
	assert(rpl_value_get_type(cv) == rpl_type_identifier);

	rpl_unistring_t cs = rpl_identifier_get_rep(cv);
	assert(cs != NULL);

	appended = rpl_unistring_append(string, cs);
	if (appended == false) goto error;

	appended = rpl_unistring_append_char(string, rpl_unichar_space);
	if (appended == false) goto error;

	appended = rpl_unistring_append(string, rpl_keyword_BY());
	if (appended == false) goto error;

	appended = rpl_unistring_append_char(string, rpl_unichar_space);
	if (appended == false) goto error;

	rpl_value_t hv = rpl_program_get_value(program, hidx);
	assert(hv != NULL);
	assert(rpl_value_get_type(hv) == rpl_type_program);

	rpl_unistring_t hs = rpl_program_intermediate_copy_string(hv,
								  env);
	assert(hs != NULL);

	appended = rpl_unistring_append(string, hs);
	rpl_unistring_release(hs);
	if (appended == false) goto error;

	appended = rpl_unistring_append_char(string, rpl_unichar_space);
	if (appended == false) goto error;

	appended = rpl_unistring_append(string, rpl_keyword_END());
	if (appended == false) goto error;

	appended = rpl_unistring_append_char(string,
					     rpl_unichar_linefeed);
	if (appended == false) goto error;
    }

    appended = rpl_unistring_append_char(string, rpl_unichar_space);
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(string, rpl_unichar_space);
    if (appended == false) goto error;

    rpl_value_t gh = rpl_program_get_value(program, count - 2);
    assert(gh != NULL);
    assert(rpl_value_get_type(gh) == rpl_type_program);

    rpl_unistring_t ghs = rpl_program_intermediate_copy_string(gh, env);
    if (ghs == NULL) goto error;

    appended = rpl_unistring_append(string, ghs);
    rpl_unistring_release(ghs);
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(string,
					 rpl_unichar_linefeed);
    if (appended == false) goto error;

    appended = rpl_unistring_append(string, rpl_keyword_END());
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(string,
					 rpl_unichar_linefeed);
    if (appended == false) goto error;

    return string;

error:
    if (string) rpl_unistring_release(string);
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

	case rpl_program_type_CONDITION:
	    return rpl_program_CONDITION_copy_string(program, env);

	case rpl_program_type_PROTECT:
	    return rpl_program_PROTECT_copy_string(program, env);
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
