//
//  rpl_string.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_string__h__
#define __RPL__rpl_string__h__

#include "rpl_defines.h"

#include "rpl_integer.h"


RPL_HEADER_BEGIN


RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_string_new(rpl_unistring_t str)
RPL_RETURNS_RETAINED;

RPL_EXPORT
rpl_unistring_t
rpl_string_get_rep(rpl_value_t val);

// TODO: Other string operations.


RPL_HEADER_END


#endif /* __RPL__rpl_string__h__ */
