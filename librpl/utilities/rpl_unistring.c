//
//  rpl_unistring.c
//  RPL
//
//  Created by Chris Hanson on 9/8/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_unistring_internal.h"

#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>


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
{
    assert(str != NULL);

    rpl_unistring_t res = rpl_unistring_new(str_len);
    if (res) {
	size_t res_idx = 0;
	rpl_unichar_t res_ch = 0x00000000;
	int remaining = 0;

	for (size_t str_idx = 0; str_idx < str_len; str_idx++) {
	    const uint8_t byte = str[str_idx];
	    const rpl_unichar_t raw = byte;
	    switch (remaining) {
		case 0: {
		    if ((raw & 0x80) == 0x00) {
			res_ch = raw;
			res->_storage[res_idx] = res_ch;
			res_idx += 1;
			remaining = 0;
		    } else if ((raw & 0xE0) == 0xC0) {
			res_ch = ((raw & 0x1F) << 6);
			remaining = 1;
		    } else if ((raw & 0xF0) == 0xE0) {
			res_ch = ((raw & 0x0F) << 12);
			remaining = 2;
		    } else if ((raw & 0xF8) == 0xF0) {
			res_ch = ((raw & 0x07) << 18);
			remaining = 3;
		    } else {
			/* Invalid sequence */
			goto error;
		    }
		} break;

		case 1: {
		    if ((raw & 0xC0) == 0x80) {
			res_ch |= (raw & 0x3F);
			res->_storage[res_idx] = res_ch;
			res_idx += 1;
			remaining = 0;
		    } else {
			/* Invalid sequence */
			goto error;
		    }
		} break;

		case 2: {
		    if ((raw & 0xC0) == 0x80) {
			res_ch |= ((raw & 0x3F) << 6);
			remaining = 1;
		    } else {
			/* Invalid sequence */
			goto error;
		    }
		} break;

		case 3: {
		    if ((raw & 0xC0) == 0x80) {
			res_ch |= ((raw & 0x3F) << 12);
			remaining = 2;
		    } else {
			/* Invalid sequence */
			goto error;
		    }
		} break;

		default: {
		    /* Logic error. */
		    assert((remaining >= 0) && (remaining <= 3));
		} break;
	    }
	}

	res->_count = res_idx;
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
{
    assert(str != NULL);

    if (str->_refcnt != INT_MAX) {
	str->_refcnt += 1;
    }

    return str;
}

void
rpl_unistring_release(rpl_unistring_t str)
{
    assert(str != NULL);

    if (str->_refcnt != INT_MAX) {
	str->_refcnt -= 1;
    }

    if (str->_refcnt == 0) {
	rpl_unistring_free(str);
    }
}

rpl_unistring_t RPL_NULLABLE
rpl_unistring_copy(rpl_unistring_t str)
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

    const size_t utf8_max = str->_count * 4 + 1;
    char *utf8 = calloc(utf8_max, sizeof(char));

    size_t utf8_idx = 0;
    for (size_t str_idx = 0; str_idx < str->_count; str_idx++) {
	rpl_unichar_t str_ch = str->_storage[str_idx];

	if (str_ch < 0x80) {
	    utf8[utf8_idx++] = (char) (str_ch & 0x7F);
	} else if ((str_ch >= 0x80) && (str_ch < 0x800)) {
	    utf8[utf8_idx++] = 0xC0 | ((str_ch & 0x07C0) >> 6);
	    utf8[utf8_idx++] = 0x80 | (str_ch & 0x003F);
	} else if ((str_ch >= 0x800) && (str_ch < 0x10000)) {
	    utf8[utf8_idx++] = 0xE0 | ((str_ch & 0xF000) >> 12);
	    utf8[utf8_idx++] = 0x80 | ((str_ch & 0x0FC0) >> 6);
	    utf8[utf8_idx++] = 0x80 | (str_ch & 0x003F);
	} else {
	    utf8[utf8_idx++] = 0xF0 | ((str_ch & 0x1C0000) >> 18);
	    utf8[utf8_idx++] = 0x80 | ((str_ch & 0x03F000) >> 12);
	    utf8[utf8_idx++] = 0x80 | ((str_ch & 0x000FC0) >> 6);
	    utf8[utf8_idx++] = 0x80 | (str_ch & 0x00003F);
	}
    }

    utf8[utf8_idx] = '\0';

    return realloc(utf8, utf8_idx + 1);
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


RPL_SOURCE_END
