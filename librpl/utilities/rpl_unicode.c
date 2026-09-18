//
//  rpl_unicode.c
//  librpl
//
//  Created by Chris Hanson on 9/16/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_unicode.h"

#include <assert.h>
#include <errno.h>
#include <iconv.h>
#include <stdlib.h>
#include <unicode/ustring.h>


RPL_SOURCE_BEGIN


bool rpl_unicode_initialized = false;

void
rpl_unicode_terminate(void);


#define rpl_iconv_err ((iconv_t)-1)
iconv_t rpl_iconv_from_utf8 = rpl_iconv_err;
iconv_t rpl_iconv_to_utf8 = rpl_iconv_err;

bool
rpl_unicode_initialize(void)
{
    if (rpl_unicode_initialized == false) {
	rpl_iconv_from_utf8 = iconv_open("UCS-4LE", "UTF-8");
	if (rpl_iconv_from_utf8 == rpl_iconv_err) goto error;

	rpl_iconv_to_utf8 = iconv_open("UTF-8", "UCS-4LE");
	if (rpl_iconv_to_utf8 == rpl_iconv_err) goto error;

	rpl_unicode_initialized = true;
    }

    return rpl_unicode_initialized;

error:
    rpl_unicode_terminate();

    return false;
}

void
rpl_unicode_terminate(void)
{
    /*
     Don't check rpl_unicode_initialized, so as to handle any failures
     during initialization.
     */

    if (rpl_iconv_from_utf8 != rpl_iconv_err) {
	(void) iconv_close(rpl_iconv_from_utf8);
	rpl_iconv_from_utf8 = rpl_iconv_err;
    }

    if (rpl_iconv_to_utf8 != rpl_iconv_err) {
	(void) iconv_close(rpl_iconv_to_utf8);
	rpl_iconv_to_utf8 = rpl_iconv_err;
    }

    rpl_unicode_initialized = false;
}

bool
rpl_unicode_convert_from_utf8(void *utf8, size_t utf8_size,
			      void *ucs4, size_t *ucs4_size)
{
    assert(utf8 != NULL);
    assert(ucs4 != NULL);
    assert(ucs4_size != NULL);

    bool initialized = rpl_unicode_initialize();
    assert(initialized);

    char *src = utf8;
    size_t src_rem = utf8_size;
    char *dst = ucs4;
    size_t dst_rem = *ucs4_size;

    size_t converted = iconv(rpl_iconv_from_utf8,
			     &src, &src_rem,
			     &dst, &dst_rem);

    *ucs4_size -= dst_rem;

    return (converted == 0);

error:
    return false;
}

bool
rpl_unicode_convert_to_utf8(void *ucs4, size_t ucs4_size,
			    void *utf8, size_t *utf8_size)
{
    assert(ucs4 != NULL);
    assert(utf8 != NULL);
    assert(utf8_size != NULL);

    bool initialized = rpl_unicode_initialize();
    assert(initialized);

    char *src = ucs4;
    size_t src_rem = ucs4_size;
    char *dst = utf8;
    size_t dst_rem = *utf8_size;

    size_t converted = iconv(rpl_iconv_to_utf8,
			     &src, &src_rem,
			     &dst, &dst_rem);

    *utf8_size -= dst_rem;

    return (converted == 0);

error:
    return false;
}

/*! Convert a UCS-4/UTF-32 string to a Uchar string for ICU use. */
bool
rpl_unicode_convert_to_UChars(void *utf32, size_t utf32_count,
			      UChar **utf16, size_t *utf16_count)
{
    assert(utf32 != NULL);
    assert(utf32_count <= INT32_MAX);
    assert(utf16 != NULL);
    assert(utf16_count != NULL);

    UChar *utf16_buf = NULL;
    int32_t utf16_buf_count = 0;

    UErrorCode icu_err = U_ZERO_ERROR;

    (void) u_strFromUTF32(NULL, 0, &utf16_buf_count,
			  utf32, (int32_t) utf32_count,
			  &icu_err);
    /*
     No need to check error, just getting what ICU thinks the size of
     the UChar buffer needs to be.
     */
    icu_err = U_ZERO_ERROR;

    utf16_buf = calloc(utf16_buf_count, sizeof(UChar));
    if (utf16_buf == NULL) goto error;

    (void) u_strFromUTF32(utf16_buf, utf16_buf_count, &utf16_buf_count,
			  utf32, (int32_t)utf32_count,
			  &icu_err);
    if (U_FAILURE(icu_err)) goto error;

    *utf16 = utf16_buf;
    *utf16_count = utf16_buf_count;

    return true;

error:
    if (utf16_buf) free(utf16_buf);
    return false;
}

bool
rpl_unicode_is_equal_case_folded(void *a, size_t a_count,
				 void *b, size_t b_count)
{
    assert(a != NULL);
    assert(b != NULL);

    if (a == b) return true;
    if ((a_count == 0) && (b_count == 0)) return true;

    UChar *a_icu = NULL;
    size_t a_icu_count = 0;
    UChar *b_icu = NULL;
    size_t b_icu_count = 0;
    bool converted;

    converted = rpl_unicode_convert_to_UChars(a, a_count,
					      &a_icu, &a_icu_count);
    if (converted == false) goto error;
    assert(a_icu_count <= INT32_MAX);

    converted = rpl_unicode_convert_to_UChars(b, b_count,
					      &b_icu, &b_icu_count);
    if (converted == false) goto error;
    assert(b_icu_count <= INT32_MAX);

    UErrorCode icu_err = U_ZERO_ERROR;
    int32_t result = u_strCaseCompare(a_icu, (int32_t)a_icu_count,
				      b_icu, (int32_t)b_icu_count,
				      U_FOLD_CASE_DEFAULT, &icu_err);
    if (U_FAILURE(icu_err)) goto error;

    bool is_equal = (result == 0);

    free(a_icu);
    free(b_icu);

    return is_equal;

error:
    if (a_icu) free(a_icu);
    if (b_icu) free(b_icu);
    return false;

}


RPL_SOURCE_END
