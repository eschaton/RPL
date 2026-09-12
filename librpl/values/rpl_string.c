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
rpl_string_new(rpl_unistring_t rep)
RPL_RETURNS_RETAINED
{
    assert(rep != NULL);

    rpl_value_t string = rpl_value_new(rpl_type_string);
    if (string) {
	string->_reps._string._str = rpl_unistring_copy(rep);
	if (string->_reps._string._str == NULL) goto error;
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

    rpl_string_t *rep = &string->_reps._string;

    rpl_unistring_release(rep->_str);
}

rpl_value_t RPL_NULLABLE
rpl_string_copy(rpl_value_t string)
RPL_RETURNS_RETAINED
{
    assert(string != NULL);
    assert(string->_type == rpl_type_string);

    rpl_string_t *rep = &string->_reps._string;

    return rpl_string_new(rep->_str);
}

rpl_unistring_t
rpl_string_get_rep(rpl_value_t string)
{
    assert(string != NULL);
    assert(string->_type == rpl_type_string);

    rpl_string_t *rep = &string->_reps._string;

    return rep->_str;
}

rpl_integer_t
rpl_string_get_rep_len(rpl_value_t string)
{
    assert(string != NULL);
    assert(string->_type == rpl_type_string);

    rpl_string_t *rep = &string->_reps._string;

    return rpl_unistring_get_length(rep->_str);
}

rpl_unistring_t RPL_NULLABLE
rpl_string_copy_string(rpl_value_t string,
		       rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED
{
    assert(string != NULL);
    assert(string->_type == rpl_type_string);

    rpl_string_t *rep = &string->_reps._string;

    /* "string" */
    rpl_unichar_t quote_arr[] = { '"' };

    rpl_unistring_t buf = rpl_unistring_new_from_chars(quote_arr, 1);
    if (buf == NULL) goto error;

    bool appended = rpl_unistring_append(buf, rep->_str);
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(buf, '"');
    if (appended == false) goto error;

    return buf;

error:
    if (buf) rpl_unistring_release(buf);
    return NULL;
}


RPL_SOURCE_END
