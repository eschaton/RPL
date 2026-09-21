//
//  rpl_identifier_internal.h
//  librpl
//
//  Created by Chris Hanson on 9/21/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_identifier_internal__h__
#define __RPL__rpl_identifier_internal__h__

#include "rpl_identifier.h"


RPL_HEADER_BEGIN


struct rpl_identifier {
    rpl_unistring_t _str;
};
typedef struct rpl_identifier rpl_identifier_t;


RPL_EXPORT
void
rpl_identifier_free(rpl_value_t identifier);

RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_identifier_copy_string(rpl_value_t val, rpl_environment_t env)
RPL_RETURNS_RETAINED;


RPL_HEADER_END


#endif /* __RPL__rpl_identifier_internal__h__ */
