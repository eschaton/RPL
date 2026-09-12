//
//  rpl_name_internal.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_name_internal__h__
#define __RPL__rpl_name_internal__h__

#include "rpl_name.h"


RPL_HEADER_BEGIN


struct rpl_name {
    rpl_unistring_t _name_text;
};
typedef struct rpl_name rpl_name_t;

RPL_EXPORT
void
rpl_name_free(rpl_value_t name);

RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_name_copy_name_text(rpl_value_t name,
			rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED;

RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_name_copy_string(rpl_value_t name,
		     rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED;


RPL_HEADER_END


#endif /* __RPL__rpl_name_internal__h__ */
