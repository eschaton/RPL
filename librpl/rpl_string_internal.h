//
//  rpl_string_internal.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_string.h"


#ifndef __RPL__rpl_string_internal__h__
#define __RPL__rpl_string_internal__h__


RPL_HEADER_BEGIN


struct rpl_string {
    char *_chars;
    rpl_integer_t _chars_len;
};
typedef struct rpl_string rpl_string_t;

RPL_EXPORT
void
rpl_string_free(rpl_value_t string);

RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_string_copy(rpl_value_t string);


RPL_HEADER_END


#endif /* __RPL__rpl_string_internal__h__ */
