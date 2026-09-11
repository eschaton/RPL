//
//  rpl_token.h
//  librpl
//
//  Created by Chris Hanson on 9/4/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_token__h__
#define __RPL__rpl_token__h__

#include "rpl_defines.h"

#include "rpl_value.h"



RPL_HEADER_BEGIN


/*! A token is a unit of RPL syntax. */
typedef struct rpl_token *rpl_token_t;


/*! Types of tokens in RPL. */
typedef enum rpl_token_type {
    rpl_token_type_unknown = 0,
    rpl_token_type_value,
    rpl_token_type_identifier,
} rpl_token_type_t;


RPL_EXPORT
void
rpl_token_free(rpl_token_t token);

RPL_EXPORT
rpl_token_type_t
rpl_token_get_type(rpl_token_t token);

RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_token_get_string(rpl_token_t token);

RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_token_get_value(rpl_token_t token);


RPL_HEADER_END


#endif /* __RPL__rpl_token__h__ */
