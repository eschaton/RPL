//
//  rpl_condition_internal.h
//  librpl
//
//  Created by Chris Hanson on 10/9/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_condition.h"

#include "rpl_scope.h"

#ifndef __RPL__rpl_condition_internal__h__
#define __RPL__rpl_condition_internal__h__


RPL_HEADER_BEGIN


typedef struct rpl_condition {
    rpl_unistring_t _name;
    rpl_scope_t _payload;
} rpl_condition_t;


RPL_EXPORT
void
rpl_condition_free(rpl_value_t condition);

RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_condition_copy_string(rpl_value_t condition, rpl_environment_t env);

/*! Initialize the condition system and standard conditions. */
RPL_EXPORT
bool
rpl_condition_initialize(void);


RPL_HEADER_END


#endif /* __RPL__rpl_condition_internal__h__ */
