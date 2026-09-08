//
//  rpl_name.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_name__h__
#define __RPL__rpl_name__h__

#include "rpl_value.h"

#include "rpl_integer.h"


RPL_HEADER_BEGIN


RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_name_new(rpl_unistring_t name_text);

RPL_EXPORT
rpl_unistring_t
rpl_name_get_rep(rpl_value_t val);


RPL_HEADER_END


#endif /* __RPL__rpl_name__h__ */
