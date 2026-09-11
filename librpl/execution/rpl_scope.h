//
//  rpl_scope.h
//  librpl
//
//  Created by Chris Hanson on 9/10/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_scope__h__
#define __RPL__rpl_scope__h__

#include "rpl_defines.h"

#include "rpl_value.h"
#include "rpl_unistring.h"


RPL_HEADER_BEGIN


/*! A scope in which variables exist. */
typedef struct rpl_scope *rpl_scope_t;


RPL_EXPORT
rpl_scope_t RPL_NULLABLE
rpl_scope_new(rpl_scope_t RPL_NULLABLE parent);

RPL_EXPORT
void
rpl_scope_free(rpl_scope_t scope);

RPL_EXPORT
rpl_scope_t RPL_NULLABLE
rpl_scope_get_parent(rpl_scope_t scope);

RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_scope_get_variable(rpl_scope_t scope, rpl_unistring_t name);

RPL_EXPORT
bool
rpl_scope_set_variable(rpl_scope_t scope, rpl_unistring_t name,
		       rpl_value_t value);


RPL_HEADER_END


#endif /* __RPL__rpl_scope__h__ */
