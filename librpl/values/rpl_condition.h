//
//  rpl_condition.h
//  librpl
//
//  Created by Chris Hanson on 10/9/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_condition__h__
#define __RPL__rpl_condition__h__

#include "rpl_value.h"

#include <stdarg.h>


RPL_HEADER_BEGIN


/*!
 Returns a newly-allocated condition.

 Additional payload can be supplied in the form of key-value pairs,
 terminated with `NULL`.
 */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_condition_new(rpl_unistring_t name, ...);

RPL_EXPORT
rpl_unistring_t
rpl_condition_get_name(rpl_value_t condition);

RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_condition_get_payload_value(rpl_value_t condition, rpl_value_t key);


RPL_HEADER_END


#endif /* __RPL__rpl_condition__h__ */
