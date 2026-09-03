//
//  rpl_environment.h
//  librpl
//
//  Created by Chris Hanson on 9/2/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_environment__h__
#define __RPL__rpl_environment__h__

#include "rpl_defines.h"


RPL_HEADER_BEGIN


/*! An environment is a context in which RPL execution takes place. */
typedef struct rpl_environment *rpl_environment_t;


/*! The base used for printing and (unadorned) reading of integers. */
typedef enum rpl_base {
    rpl_base_binary      = 2,
    rpl_base_octal       = 8,
    rpl_base_decimal     = 10,
    rpl_base_hexadecimal = 16,
} rpl_base_t;


RPL_EXPORT
rpl_environment_t RPL_NULLABLE
rpl_environment_new(void);

RPL_EXPORT
void
rpl_environment_free(rpl_environment_t env);

RPL_EXPORT
rpl_base_t
rpl_environment_get_base(rpl_environment_t env);

RPL_EXPORT
void
rpl_environment_set_base(rpl_environment_t env, rpl_base_t base);


RPL_HEADER_END


#endif /* __RPL__rpl_environment__h__ */
