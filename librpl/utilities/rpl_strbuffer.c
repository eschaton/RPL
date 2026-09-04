//
//  rpl_strbuffer.c
//  RPL
//
//  Created by Chris Hanson on 9/2/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_strbuffer_internal.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "rpl_adjbuffer.h"


RPL_SOURCE_BEGIN


rpl_strbuffer_t RPL_NULLABLE
rpl_strbuffer_new_empty(size_t capacity)
{
    rpl_strbuffer_t sb = calloc(1, sizeof(struct rpl_strbuffer));
    if (sb) {
	bool initialized
	    = rpl_adjbuffer_init(&sb->_ab, capacity, sizeof(char));
	if (initialized == false) goto error;
    }
    return sb;

error:
    rpl_strbuffer_free(sb);
    return NULL;
}

rpl_strbuffer_t RPL_NULLABLE
rpl_strbuffer_new(const char *str)
{
    assert(str != NULL);

    const size_t str_len = strlen(str);
    rpl_strbuffer_t sb = rpl_strbuffer_new_empty(str_len + 1);
    if (sb) {
	bool appended = rpl_strbuffer_append_chars(sb, str);
	if (appended == false) goto error;
    }
    return sb;

error:
    rpl_strbuffer_free(sb);
    return NULL;
}

void
rpl_strbuffer_free(rpl_strbuffer_t sb)
{
    assert(sb != NULL);

    rpl_adjbuffer_deinit(&sb->_ab);
    free(sb);
}

const char *
rpl_strbuffer_get_chars(rpl_strbuffer_t sb)
{
    assert(sb != NULL);

    return sb->_ab._storage;
}

const char * RPL_NULLABLE
rpl_strbuffer_copy_chars(rpl_strbuffer_t sb)
{
    assert(sb != NULL);

    const size_t count = rpl_adjbuffer_get_count(&sb->_ab);
    char *chars = calloc(count, sizeof(char));
    if (chars) {
	strlcpy(chars, rpl_strbuffer_get_chars(sb), count);
    }
    return chars;
}

size_t
rpl_strbuffer_get_length(rpl_strbuffer_t sb)
{
    assert(sb != NULL);

    /*
     The buffer count includes the trailing NUL character, which
     shouldn't be included in a C string length.
     */

    return rpl_adjbuffer_get_count(&sb->_ab) - 1;
}

bool
rpl_strbuffer_append_chars(rpl_strbuffer_t sb, const char *str)
{
    assert(sb != NULL);
    assert(str != NULL);

    /*
     Remove the trailing NUL character and then append, which will add
     the trailing NUL character from str.
     */

    const size_t str_len = strlen(str);
    const size_t sb_len = rpl_strbuffer_get_length(sb);
    rpl_adjbuffer_remove_element(&sb->_ab, sb_len);
    return rpl_adjbuffer_append_elements(&sb->_ab, (void *)str,
					 str_len + 1);
}

bool
rpl_strbuffer_append_strbuffer(rpl_strbuffer_t sb,
			       rpl_strbuffer_t sb_appended)
{
    assert(sb != NULL);
    assert(sb_appended != NULL);

    /*
     Remove the trailing NUL character and then append, which will add
     the trailing NUL character from sb_appended.
     */

    const char *sb_appended_chars
	= rpl_strbuffer_get_chars(sb_appended);
    const size_t sb_appended_len
	= rpl_strbuffer_get_length(sb_appended) + 1;
    const size_t sb_len = rpl_strbuffer_get_length(sb);
    rpl_adjbuffer_remove_element(&sb->_ab, sb_len);
    return rpl_adjbuffer_append_elements(&sb->_ab, sb_appended_chars,
					 sb_appended_len + 1);
}


RPL_SOURCE_END
