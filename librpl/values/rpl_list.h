//
//  rpl_list.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_list__h__
#define __RPL__rpl_list__h__

#include "rpl_defines.h"

#include "rpl_integer.h"
#include "rpl_value.h"


RPL_HEADER_BEGIN


RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_list_new(void)
RPL_RETURNS_RETAINED;

RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_list_new_with_pair(rpl_value_t RPL_NULLABLE car,
		       rpl_value_t RPL_NULLABLE cdr)
RPL_RETURNS_RETAINED;

RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_list_new_with_array(rpl_value_t RPL_NONNULL * RPL_NONNULL elements,
			rpl_integer_t elements_count)
RPL_RETURNS_RETAINED;

RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_list_car(rpl_value_t val);

RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_list_cdr(rpl_value_t val);

RPL_EXPORT
void
rpl_list_append(rpl_value_t list, rpl_value_t list2);


RPL_HEADER_END


#endif /* __RPL__rpl_list__h__ */
