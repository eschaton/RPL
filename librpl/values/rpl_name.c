//
//  rpl_name.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_name_internal.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN


/*
 TODO: Name interning mechanism

 Names should be interned and reference counted, so using the same names
 in a lot of different situations simultaneously is efficient.
 */


rpl_value_t RPL_NULLABLE
rpl_name_new(rpl_unistring_t name_text)
{
    assert(name_text != NULL);

    rpl_value_t name = rpl_value_new(rpl_type_name);
    if (name) {
	rpl_name_t *rep = &name->_reps._name;
	rep->_name_text = rpl_unistring_copy(name_text);
	if (rep->_name_text == NULL) goto error;
    }
    return name;

error:
    rpl_value_release(name);
    return NULL;
}

void
rpl_name_free(rpl_value_t name)
{
    assert(name != NULL);
    assert(name->_type == rpl_type_name);

    rpl_name_t *rep = &name->_reps._name;

    rpl_unistring_release(rep->_name_text);
}

rpl_unistring_t
rpl_name_get_rep(rpl_value_t name)
{
    assert(name != NULL);
    assert(name->_type == rpl_type_name);

    rpl_name_t *rep = &name->_reps._name;

    return rep->_name_text;
}

rpl_unistring_t RPL_NULLABLE
rpl_name_copy_name_text(rpl_value_t name,
			rpl_environment_t RPL_NULLABLE env)
{
    assert(name != NULL);

    rpl_name_t *rep = &name->_reps._name;

    return rpl_unistring_copy(rep->_name_text);
}

rpl_unistring_t RPL_NULLABLE
rpl_name_copy_string(rpl_value_t name,
		     rpl_environment_t RPL_NULLABLE env)
{
    assert(name != NULL);
    assert(name->_type == rpl_type_name);

    rpl_name_t *rep = &name->_reps._name;

    /* 'name' */
    rpl_unichar_t quote_arr[] = { '\'' };

    rpl_unistring_t buf = rpl_unistring_new_from_chars(quote_arr, 1);
    if (buf == NULL) goto error;

    bool appended = rpl_unistring_append(buf, rep->_name_text);
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(buf, '\'');
    if (appended == false) goto error;

    return buf;

error:
    if (buf) rpl_unistring_release(buf);
    return NULL;
}


RPL_SOURCE_END
