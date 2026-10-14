//
//  rpl_keyword.h
//  librpl
//
//  Created by Chris Hanson on 9/26/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_keyword__h__
#define __RPL__rpl_keyword__h__

#include "rpl_defines.h"

#include "rpl_unistring.h"


RPL_HEADER_BEGIN

/*
 RPL has a minimal set of "true" keywords that are used to represent
 control flow, but it also has a number of other strings that are used
 in a similar manner to keywords.

 These all exist as immortal strings called keywords for ease and speed
 of comparison.
 */

#define RPL_DECLARE_KEYWORD(name) \
RPL_EXPORT \
rpl_unistring_t \
rpl_keyword_ ## name (void)


RPL_DECLARE_KEYWORD(DO);
RPL_DECLARE_KEYWORD(UNTIL);
RPL_DECLARE_KEYWORD(IF);
RPL_DECLARE_KEYWORD(THEN);
RPL_DECLARE_KEYWORD(ELSE);
RPL_DECLARE_KEYWORD(FOR);
RPL_DECLARE_KEYWORD(NEXT);
RPL_DECLARE_KEYWORD(STEP);
RPL_DECLARE_KEYWORD(CASE);
RPL_DECLARE_KEYWORD(START);
RPL_DECLARE_KEYWORD(WHILE);
RPL_DECLARE_KEYWORD(REPEAT);
RPL_DECLARE_KEYWORD(END);

RPL_DECLARE_KEYWORD(CONDITION);
RPL_DECLARE_KEYWORD(NAME);
RPL_DECLARE_KEYWORD(OPERATION);
RPL_DECLARE_KEYWORD(EXPECTED);
RPL_DECLARE_KEYWORD(ACTUAL);
RPL_DECLARE_KEYWORD(MEMORY_FULL);
RPL_DECLARE_KEYWORD(INSUFFICIENT_ARGUMENTS);
RPL_DECLARE_KEYWORD(INVALID_TYPE);
RPL_DECLARE_KEYWORD(PROTECT);
RPL_DECLARE_KEYWORD(HANDLE);
RPL_DECLARE_KEYWORD(BY);


RPL_HEADER_END


#endif /* __RPL__rpl_keyword__h__ */
