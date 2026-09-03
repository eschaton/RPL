//
//  rpl_string.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_string_internal.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN


rpl_value_t RPL_NULLABLE
rpl_string_new(const char *rep, rpl_integer_t rep_len)
{
    assert(rep != NULL);
    assert(strlen(rep) == rep_len);

    rpl_value_t string = rpl_value_new(rpl_type_string);
    if (string) {
	string->_reps._string._chars = strdup(rep);
	if (string->_reps._string._chars == NULL) goto error;

	string->_reps._string._chars_len = rep_len;
    }
    return string;

error:
    rpl_value_release(string);
    return NULL;
}

void
rpl_string_free(rpl_value_t string)
{
    assert(string != NULL);
    assert(string->_type == rpl_type_string);

    free(string->_reps._string._chars);
}

rpl_value_t RPL_NULLABLE
rpl_string_copy(rpl_value_t string)
{
    // TODO: rpl_string_copy

    return NULL;
}

const char *
rpl_string_get_rep(rpl_value_t string)
{
    assert(string != NULL);
    assert(string->_type == rpl_type_string);

    return string->_reps._string._chars;
}

rpl_integer_t
rpl_string_get_rep_len(rpl_value_t string)
{
    assert(string != NULL);
    assert(string->_type == rpl_type_string);

    return string->_reps._string._chars_len;
}

const char * RPL_NULLABLE
rpl_string_copy_string(rpl_value_t string,
		       rpl_environment_t RPL_NULLABLE env)
{
    assert(string != NULL);
    assert(string->_type == rpl_type_string);

    rpl_string_t *rep = &string->_reps._string;

    /* "string" */
    const size_t buf_len = 1 + rep->_chars_len + 1 + 1;
    char *buf = calloc(sizeof(char), buf_len);
    if (buf) {
	buf[0] = '"';
	strlcpy(&buf[1], rep->_chars, buf_len - 1);
	buf[buf_len - 2] = '"';
    }

    return buf;
}


RPL_SOURCE_END
