//
//  rpl_keyword.c
//  librpl
//
//  Created by Chris Hanson on 9/26/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_keyword.h"

#include <assert.h>
#include <string.h>

#include "rpl_unistring_internal.h"


RPL_SOURCE_BEGIN


#define RPL_DEFINE_KEYWORD(name) \
    rpl_unistring_t rpl_keyword_ ## name (void) \
    { \
	static rpl_unistring_t kw_ ## name = NULL; \
	if (kw_ ## name == NULL) { \
	    const char *kw = #name ; \
	    const size_t kw_len = strlen(kw); \
	    kw_ ## name = rpl_unistring_new_from_utf8(kw, kw_len); \
	    assert(kw_ ## name != NULL); \
	    rpl_unistring_immortalize(kw_ ## name); \
	} \
	return kw_ ## name; \
    }

RPL_DEFINE_KEYWORD(DO);
RPL_DEFINE_KEYWORD(UNTIL);
RPL_DEFINE_KEYWORD(IF);
RPL_DEFINE_KEYWORD(THEN);
RPL_DEFINE_KEYWORD(ELSE);
RPL_DEFINE_KEYWORD(FOR);
RPL_DEFINE_KEYWORD(NEXT);
RPL_DEFINE_KEYWORD(STEP);
RPL_DEFINE_KEYWORD(CASE);
RPL_DEFINE_KEYWORD(START);
RPL_DEFINE_KEYWORD(WHILE);
RPL_DEFINE_KEYWORD(REPEAT);
RPL_DEFINE_KEYWORD(END);


RPL_SOURCE_END
