//
//  rpl_unistring.c
//  librpl
//
//  Created by Chris Hanson on 9/8/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_unistring_internal.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rpl_unicode.h"


RPL_SOURCE_BEGIN


/*! Amount by which capacity increases, also used for base capacity. */
const size_t rpl_unistring_quantum = 16;


/*! Round to the next quantum. */
size_t
rpl_unistring_round_to_next(size_t value)
{
    if (value <= rpl_unistring_quantum) return rpl_unistring_quantum;

    return value + (rpl_unistring_quantum
		    - (value % rpl_unistring_quantum));
}

/*!
 Adjust the storage of a buffer if it's about to be filled.

 @returns `false` if allocation failed, `true` otherwise
 */
bool
rpl_unistring_adjust_storage(rpl_unistring_t str, size_t count)
{
    assert(str != NULL);

    if (str->_count + count >= str->_capacity) {
	const size_t ocap = str->_capacity;
	const size_t ncap = rpl_unistring_round_to_next(ocap + count);
	rpl_unichar_t *ostorage = str->_storage;
	rpl_unichar_t *nstorage = realloc(ostorage, ncap);
	if (nstorage == NULL) goto error;
	rpl_unichar_t *nextra = nstorage + ocap;
	memset(nextra, 0, (ncap - ocap) * sizeof(rpl_unichar_t));
	str->_storage = nstorage;
	str->_capacity = ncap;
    }

    return true;

error:
    return false;
}

rpl_unistring_t RPL_NULLABLE
rpl_unistring_new(size_t capacity)
RPL_RETURNS_RETAINED
{
    rpl_unistring_t str = calloc(1, sizeof(struct rpl_unistring));
    if (str) {
	size_t real_capacity = rpl_unistring_round_to_next(capacity);
	str->_storage = calloc(real_capacity, sizeof(rpl_unichar_t));
	if (str->_storage == NULL) goto error;
	str->_count = 0;
	str->_capacity = real_capacity;
	str->_refcnt = 1;
    }
    return str;

error:
    rpl_unistring_free(str);
    return NULL;
}

rpl_unistring_t RPL_NULLABLE
rpl_unistring_new_from_chars(const rpl_unichar_t *chars,
			     size_t chars_count)
RPL_RETURNS_RETAINED
{
    assert(chars != NULL);

    rpl_unistring_t str = rpl_unistring_new(chars_count);
    if (str) {
	memcpy(str->_storage, chars,
	       chars_count * sizeof(rpl_unichar_t));
	str->_count = chars_count;
    }
    return str;
}

rpl_unistring_t RPL_NULLABLE
rpl_unistring_new_from_utf8(const char *str, size_t str_len)
RPL_RETURNS_RETAINED
{
    assert(str != NULL);

    rpl_unistring_t res = rpl_unistring_new(str_len);
    if (res) {
	size_t stsize = res->_capacity * sizeof(rpl_unichar_t);
	bool converted
	    = rpl_unicode_convert_from_utf8((void *)str, str_len,
					    res->_storage, &stsize);
	if (converted == false) goto error;
	res->_count = stsize / sizeof(rpl_unichar_t);
    }

    return res;

error:
    rpl_unistring_free(res);
    return NULL;
}

void
rpl_unistring_free(rpl_unistring_t str)
{
    assert(str != NULL);

    free(str->_storage);
    free(str);
}

rpl_unistring_t
rpl_unistring_retain(rpl_unistring_t str)
RPL_RETURNS_RETAINED
{
    assert(str != NULL);

    if (str->_refcnt != INT_MAX) {
	str->_refcnt += 1;
    }

    return str;
}

void
rpl_unistring_release(rpl_unistring_t str RPL_RELEASES_ARGUMENT)
{
    assert(str != NULL);

    if (str->_refcnt != INT_MAX) {
	str->_refcnt -= 1;
    }

    if (str->_refcnt == 0) {
	rpl_unistring_free(str);
    }
}

void
rpl_unistring_immortalize(rpl_unistring_t str)
{
    assert(str != NULL);
    assert(str->_refcnt != INT_MAX);

    str->_refcnt = INT_MAX;
}

void
rpl_unistring_not_leaked(rpl_unistring_t str RPL_RELEASES_ARGUMENT)
{
    assert(str != NULL);

    /*
     Nothing else to do: This function exists purely to supply its
     argument annotation to the static analyzer. (This is mainly for
     the situation where an "RPL_RETAINS_ARGUMENT" annotation would be
     useful, but such a thing doesn't exist.)
     */
}

rpl_unistring_t RPL_NULLABLE
rpl_unistring_copy(rpl_unistring_t str)
RPL_RETURNS_RETAINED
{
    assert(str != NULL);

    rpl_unistring_t copy = calloc(1, sizeof(struct rpl_unistring));
    if (copy) {
	copy->_storage = calloc(str->_capacity, sizeof(rpl_unichar_t));
	if (copy->_storage == NULL) goto error;
	memcpy(copy->_storage, str->_storage,
	       str->_count * sizeof(rpl_unichar_t));
	copy->_count = str->_count;
	copy->_capacity = str->_capacity;
	copy->_refcnt = 1;
    }

    return copy;

error:
    rpl_unistring_free(copy);
    return NULL;
}

size_t
rpl_unistring_get_length(rpl_unistring_t str)
{
    assert(str != NULL);

    return str->_count;
}

rpl_unichar_t
rpl_unistring_get_char(rpl_unistring_t str, size_t idx)
{
    assert(str != NULL);
    assert(idx < str->_count);

    return str->_storage[idx];
}

void
rpl_unistring_set_char(rpl_unistring_t str, size_t idx,
		       rpl_unichar_t ch)
{
    assert(str != NULL);
    assert(idx < str->_count);

    str->_storage[idx] = ch;
}

bool
rpl_unistring_append(rpl_unistring_t a, rpl_unistring_t b)
{
    assert(a != NULL);
    assert(b != NULL);

    bool adjusted = rpl_unistring_adjust_storage(a, b->_count);
    if (adjusted == false) goto error;

    memcpy(&a->_storage[a->_count], b->_storage,
	   b->_count * sizeof(rpl_unichar_t));
    a->_count += b->_count;

    return true;

error:
    return false;
}

bool
rpl_unistring_append_char(rpl_unistring_t str, rpl_unichar_t ch)
{
    assert(str != NULL);

    bool adjusted = rpl_unistring_adjust_storage(str, 1);
    if (adjusted == false) goto error;

    str->_storage[str->_count] = ch;
    str->_count += 1;

    return true;

error:
    return false;
}

void
rpl_unistring_remove(rpl_unistring_t str, size_t idx)
{
    assert(str != NULL);
    assert(idx < str->_count);
    assert(str->_count > 0);

    for (size_t i = idx; i < (str->_count - 1); i++) {
	str->_storage[i] = str->_storage[i+1];
    }

    str->_count -= 1;
}

void
rpl_unistring_remove_all(rpl_unistring_t str)
{
    assert(str != NULL);

    str->_count = 0;
    memset(str->_storage, 0, str->_capacity * sizeof(rpl_unichar_t));
}

char * RPL_NULLABLE
rpl_unistring_copy_utf8(rpl_unistring_t str)
{
    assert(str != NULL);

    const size_t ucs4_size = str->_count * sizeof(rpl_unichar_t);
    const size_t utf8_max = str->_count * 6;
    char *utf8 = calloc(utf8_max, sizeof(char));
    if (utf8 == NULL) goto error;

    size_t utf8_size = utf8_max;

    bool converted
	= rpl_unicode_convert_to_utf8(str->_storage, ucs4_size,
				      utf8, &utf8_size);
    if (converted == false) goto error;

    char *result = realloc(utf8, utf8_size + 1);
    if (result == NULL) goto error;

    result[utf8_size] = '\0';

    return result;

error:
    free(utf8);
    return NULL;
}

int
rpl_unistring_compare(rpl_unistring_t a, rpl_unistring_t b)
{
    assert(a != NULL);
    assert(b != NULL);

    if (a->_count < b->_count) {
	return -1;
    } else if (a->_count > b->_count) {
	return 1;
    } else /* a->_count == b->_count */ {
	return memcmp(a->_storage, b->_storage,
		      a->_count * sizeof(rpl_unichar_t));
    }
}

rpl_unistring_t
rpl_unistring_get_eol(void)
{
    static rpl_unistring_t rpl_unistring_eol = NULL;

    if (rpl_unistring_eol == NULL) {
	rpl_unichar_t ch = { rpl_unichar_linefeed };
	rpl_unistring_eol = rpl_unistring_new_from_chars(&ch, 1);
	assert(rpl_unistring_eol != NULL);
	rpl_unistring_immortalize(rpl_unistring_eol);
    }

    return rpl_unistring_eol;
}

rpl_unistring_t
rpl_unistring_with_digit(int d)
{
    assert((d >= 0) && (d <= 9));

    static rpl_unistring_t rpl_unistring_digits[10] = { NULL };

    if (rpl_unistring_digits[d] == NULL) {
	rpl_unichar_t dch = { '0' + d };
	rpl_unistring_digits[d] = rpl_unistring_new_from_chars(&dch, 1);
	assert(rpl_unistring_digits[d] != NULL);
	rpl_unistring_immortalize(rpl_unistring_digits[d]);
    }

    return rpl_unistring_digits[d];
}

rpl_unistring_t RPL_NULLABLE
rpl_unistring_with_integer(int64_t i)
RPL_RETURNS_RETAINED
{
    char buf[64] = {0};

    int len = snprintf(buf, 64, "%llu", i);
    if (len < 0) return NULL;

    return rpl_unistring_new_from_utf8(buf, len);
}


RPL_SOURCE_END
