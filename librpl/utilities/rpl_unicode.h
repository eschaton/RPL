//
//  rpl_iconv.h
//  librpl
//
//  Created by Chris Hanson on 9/16/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_iconv__h__
#define __RPL__rpl_iconv__h__

#include "rpl_defines.h"

#include <stdbool.h>
#include <unistd.h>


RPL_HEADER_BEGIN

/*! Initialize the Unicode support subsystem. */
RPL_EXPORT
bool
rpl_unicode_initialize(void);


/*
 The iconv(3) function works in terms of source and destination buffers
 that may be only partially consumed or filled, if the conversion
 encounters errors. Since RPL works in terms of UTF-8 for external
 strings and UCS-4 for internal strings, this is the sort of thing that
 shouldn't happen. It's a lot more convenient to just work in terms of
 initial and eventual buffer sizes.
 */


/*!
 A convenience wrapper for ``iconv(3)`` to convert from UTF-8 to the RPL
 internal representation (native-endian/alignment UCS-4).

 @param utf8 the UTF-8 input text buffer
 @param utf8_size the size _in bytes_ of the input text
 @param ucs4 the UCS-4 output text buffer
 @param ucs4_size on entry, the size _in bytes_ of the output text
		  buffer; on successful exit, the size _in bytes_ used
 @returns `true` on success, `false` on failure.
 */
RPL_EXPORT
bool
rpl_unicode_convert_from_utf8(void *utf8, size_t utf8_size,
			      void *ucs4, size_t *ucs4_size);

/*!
 A convenience wrapper for ``iconv(3)`` to convert from the RPL internal
 representation (native-endian/alignment UCS-4) to UTF-8.

 @param ucs4 the UCS-4 input text buffer
 @param ucs4_size the size _in bytes_ of the input text
 @param utf8 the UTF-8 output text buffer
 @param utf8_size on entry, the size _in bytes_ of the output text
		  buffer; on successful exit, the size _in bytes_ used
 @returns `true` on success, `false` on failure.
 */
RPL_EXPORT
bool
rpl_unicode_convert_to_utf8(void *ucs4, size_t ucs4_size,
			    void *utf8, size_t *utf8_size);

/*!
 A convenience wrapper for ``u_strCaseCompare`` from ICU, to perform a
 case-folded identity comparison against two UCS-4 strings.

 - NOTE: This SPI uses _code point counts_ rather than _byte sizes_.
 */
RPL_EXPORT
bool
rpl_unicode_is_equal_case_folded(void *a, size_t a_count,
				 void *b, size_t b_count);


RPL_HEADER_END


#endif /* __RPL__rpl_iconv__h__ */
