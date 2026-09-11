//
//  rpl_unistring.h
//  RPL
//
//  Created by Chris Hanson on 9/8/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_unistring__h__
#define __RPL__rpl_unistring__h__

#include "rpl_defines.h"

#include <stdbool.h>
#include <stdint.h>


RPL_HEADER_BEGIN

/*
 RPL internally uses UTF-32 and a counted representation of strings,
 rather than UTF-8 and C strings.

 RPL strings are reference-counted, mutable, and support copying.
 */


/*! A UTF-32 code point. */
typedef uint32_t rpl_unichar_t;


/*! A string made up of UTF-32 code points. */
typedef struct rpl_unistring *rpl_unistring_t;


RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_unistring_new(size_t capacity);

RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_unistring_new_from_chars(const rpl_unichar_t *chars,
			     size_t chars_count);

RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_unistring_new_from_utf8(const char *str, size_t str_len);

RPL_EXPORT
rpl_unistring_t
rpl_unistring_retain(rpl_unistring_t str);

RPL_EXPORT
void
rpl_unistring_release(rpl_unistring_t str);

RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_unistring_copy(rpl_unistring_t str);

RPL_EXPORT
size_t
rpl_unistring_get_length(rpl_unistring_t str);

RPL_EXPORT
rpl_unichar_t
rpl_unistring_get_char(rpl_unistring_t str, size_t idx);

RPL_EXPORT
void
rpl_unistring_set_char(rpl_unistring_t str, size_t idx,
		       rpl_unichar_t ch);

RPL_EXPORT
bool
rpl_unistring_append(rpl_unistring_t a, rpl_unistring_t b);

RPL_EXPORT
bool
rpl_unistring_append_char(rpl_unistring_t str, rpl_unichar_t ch);

RPL_EXPORT
void
rpl_unistring_remove(rpl_unistring_t str, size_t idx);

RPL_EXPORT
void
rpl_unistring_remove_all(rpl_unistring_t str);

RPL_EXPORT
char * RPL_NULLABLE
rpl_unistring_copy_utf8(rpl_unistring_t str);

/*! Compare two strings for equality, behaving like strcmp. */
RPL_EXPORT
int
rpl_unistring_compare(rpl_unistring_t a, rpl_unistring_t b);


RPL_HEADER_END


#endif /* __RPL__rpl_unistring__h__ */
