//
//  rpl_tagged.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_tagged_internal.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN


rpl_value_t RPL_NULLABLE
rpl_tagged_new(rpl_value_t tag, rpl_value_t value)
{
    assert(tag != NULL);
    assert(tag->_type == rpl_type_name);
    assert(value != NULL);

    rpl_value_t tagged = rpl_value_new(rpl_type_tagged);
    if (tagged) {
	tagged->_reps._tagged._tag = rpl_value_retain(tag);
	tagged->_reps._tagged._value = rpl_value_retain(value);
    }
    return tagged;
}

void
rpl_tagged_free(rpl_value_t tagged)
{
    assert(tagged != NULL);
    assert(tagged->_type == rpl_type_tagged);

    rpl_value_release(tagged->_reps._tagged._tag);
    rpl_value_release(tagged->_reps._tagged._value);
}

rpl_value_t
rpl_tagged_get_tag(rpl_value_t tagged)
{
    assert(tagged != NULL);
    assert(tagged->_type == rpl_type_tagged);

    return tagged->_reps._tagged._tag;
}

rpl_value_t
rpl_tagged_get_value(rpl_value_t tagged)
{
    assert(tagged != NULL);
    assert(tagged->_type == rpl_type_tagged);

    return tagged->_reps._tagged._value;
}

rpl_unistring_t RPL_NULLABLE
rpl_tagged_copy_string(rpl_value_t tagged,
		       rpl_environment_t RPL_NULLABLE env)
{
    assert(tagged != NULL);
    assert(tagged->_type == rpl_type_tagged);

    rpl_tagged_t *rep = &tagged->_reps._tagged;

    rpl_unistring_t tag_str = NULL;
    rpl_unistring_t value_str = NULL;
    rpl_unistring_t buf = NULL;
    bool appended = false;

    tag_str = rpl_name_copy_name_text(rep->_tag, env);
    if (tag_str == NULL) goto error;
    const size_t tag_str_len = rpl_unistring_get_length(tag_str);

    value_str = rpl_value_copy_string(rep->_value, env);
    if (value_str == NULL) goto error;
    const size_t value_str_len = rpl_unistring_get_length(tag_str);

    /* :tag:value */

    buf = rpl_unistring_new(tag_str_len + value_str_len + 2);
    if (buf == NULL) goto error;

    appended = rpl_unistring_append_char(buf, ':');
    if (appended == false) goto error;

    appended = rpl_unistring_append(buf, tag_str);
    if (appended == false) goto error;

    appended = rpl_unistring_append_char(buf, ':');
    if (appended == false) goto error;

    appended = rpl_unistring_append(buf, value_str);
    if (appended == false) goto error;

    rpl_unistring_release(tag_str);
    rpl_unistring_release(value_str);

    return buf;

error:
    if (tag_str) rpl_unistring_release(tag_str);
    if (value_str) rpl_unistring_release(value_str);
    if (buf) rpl_unistring_release(buf);
    return NULL;
}


RPL_SOURCE_END
