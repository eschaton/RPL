//
//  rpl_name_internal.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_name.h"


#ifndef __RPL__rpl_name_internal__h__
#define __RPL__rpl_name_internal__h__


RPL_HEADER_BEGIN


struct rpl_name {
    rpl_integer_t _chars_len;
    char *_chars;
};
typedef struct rpl_name rpl_name_t;

RPL_EXPORT
void
rpl_name_free(rpl_value_t name);


RPL_HEADER_END


#endif /* __RPL__rpl_name_internal__h__ */
