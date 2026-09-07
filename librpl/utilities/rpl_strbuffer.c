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


/*! Amount by which capacity increases, also used for base capacity. */
const size_t rpl_strbuffer_quantum = 16;


/*! Round to the next quantum. */
size_t
rpl_strbuffer_round_to_next(size_t value)
{
    return value + (rpl_strbuffer_quantum
		    - (value % rpl_strbuffer_quantum));
}

/*!
 Adjust the storage of a buffer if it's about to be filled.

 @returns `false` if allocation failed, `true` otherwise
 */
bool
rpl_strbuffer_adjust_storage(rpl_strbuffer_t sb, size_t count)
{
    assert(sb != NULL);

    if (sb->_count + count >= sb->_capacity) {
	const size_t ocap = sb->_capacity;
	const size_t ncap = rpl_strbuffer_round_to_next(ocap + count);
	const size_t osize = ocap;
	const size_t nsize = ncap;
	char *ostorage = sb->_storage;
	char *nstorage = realloc(ostorage, nsize);
	if (nstorage == NULL) goto error;
	void *nextra = nstorage + osize;
	memset(nextra, 0, nsize - osize);
	sb->_storage = nstorage;
	sb->_capacity = ncap;
    }

    return true;

error:
    return false;
}

rpl_strbuffer_t RPL_NULLABLE
rpl_strbuffer_new_empty(size_t capacity)
{
    rpl_strbuffer_t sb = calloc(1, sizeof(struct rpl_strbuffer));
    if (sb) {
	const size_t real_capacity
	    = rpl_strbuffer_round_to_next(capacity);
	sb->_storage = calloc(real_capacity, sizeof(char));
	if (sb->_storage == NULL) goto error;
	sb->_count = 1; /* always has a trailing NUL */
	sb->_capacity = real_capacity;
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
    rpl_strbuffer_t sb = rpl_strbuffer_new_empty(str_len);
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

    free(sb->_storage);
    free(sb);
}

char
rpl_strbuffer_get_char(rpl_strbuffer_t sb, size_t idx)
{
    assert(sb != NULL);
    assert(idx < (sb->_count - 1));

    const char *chars = sb->_storage;
    return chars[idx];
}

const char *
rpl_strbuffer_get_chars(rpl_strbuffer_t sb)
{
    assert(sb != NULL);

    return sb->_storage;
}

const char * RPL_NULLABLE
rpl_strbuffer_copy_chars(rpl_strbuffer_t sb)
{
    assert(sb != NULL);

    const size_t count = sb->_count;
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

    return sb->_count - 1;
}

bool
rpl_strbuffer_append_char(rpl_strbuffer_t sb, char ch)
{
    assert(sb != NULL);
    assert(ch != '\0');

    bool adjusted = rpl_strbuffer_adjust_storage(sb, 1);
    if (adjusted == false) return false;

    sb->_storage[sb->_count - 1] = ch;  /* old NUL */
    sb->_storage[sb->_count] = '\0';    /* new NUL */
    sb->_count += 1;

    return true;
}

bool
rpl_strbuffer_append_chars(rpl_strbuffer_t sb, const char *str)
{
    assert(sb != NULL);
    assert(str != NULL);

    const size_t str_len = strlen(str);
    if (str_len == 0) return true;

    bool adjusted = rpl_strbuffer_adjust_storage(sb, str_len);
    if (adjusted == false) return false;

    memcpy(&sb->_storage[sb->_count - 1], str, str_len + 1);
    sb->_count += str_len;

    return true;
}

bool
rpl_strbuffer_append_strbuffer(rpl_strbuffer_t sb,
			       rpl_strbuffer_t sb_app)
{
    assert(sb != NULL);
    assert(sb_app != NULL);

    bool adjusted = rpl_strbuffer_adjust_storage(sb, sb_app->_count);
    if (adjusted == false) return false;

    memcpy(&sb->_storage[sb->_count - 1], sb_app->_storage,
	   sb_app->_count);
    sb->_count += sb_app->_count - 1;

    return true;
}

void
rpl_strbuffer_remove_all(rpl_strbuffer_t sb)
{
    assert(sb != NULL);
    assert(sb->_capacity >= 1);

    sb->_storage[0] = '\0';
    sb->_count = 1;
}


RPL_SOURCE_END
