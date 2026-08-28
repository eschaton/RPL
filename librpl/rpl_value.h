//
//  rpl_value.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_value__h__
#define __RPL__rpl_value__h__

#include <stdbool.h>
#include <stdint.h>

#include "rpl_defines.h"


RPL_HEADER_BEGIN


/*! The types of values RPL supports. */
enum rpl_type {
    rpl_type_integer,
    rpl_type_real,
    rpl_type_complex,
    rpl_type_array,
    rpl_type_name,
    rpl_type_program,
    rpl_type_string,
    rpl_type_list,
    rpl_type_tagged,
    rpl_type_unit,
};
typedef enum rpl_type rpl_type_t;


/*! A typed value in RPL. */
typedef struct rpl_value *rpl_value_t;


/*! Get the type of a value. */
RPL_EXPORT
rpl_type_t
rpl_value_get_type(rpl_value_t val);

/*! Retain a value. */
RPL_EXPORT
void
rpl_value_retain(rpl_value_t val);

/*! Release a value. */
RPL_EXPORT
void
rpl_value_release(rpl_value_t val);


RPL_HEADER_END


#endif /* __RPL__rpl_value__h__ */
