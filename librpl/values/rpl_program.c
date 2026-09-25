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

#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN


rpl_value_t RPL_NULLABLE
rpl_program_new(void)
{
    rpl_value_t val = rpl_value_new(rpl_type_program);
    if (val) {
	rpl_program_t *rep = &val->_reps._program;
	bool inited = rpl_adjbuffer_init(&rep->_values, 8,
					 sizeof(rpl_value_t));
	if (inited == false) goto error;
    }
    return val;

error:
    rpl_value_release(val);
    return NULL;
}

void
rpl_program_free(rpl_value_t program)
{
    assert(program != NULL);
    assert(program->_type == rpl_type_program);
    rpl_program_t *rep = &program->_reps._program;

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

    return rpl_adjbuffer_append_element(&rep->_values, &value);
}

rpl_unistring_t RPL_NULLABLE
rpl_program_copy_string(rpl_value_t program,
			rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED
{
    assert(program != NULL);
    assert(program->_type == rpl_type_program);

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


RPL_SOURCE_END
