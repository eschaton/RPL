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


RPL_SOURCE_END
