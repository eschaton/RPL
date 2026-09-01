//
//  rpl_array_internal.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_array_internal__h__
#define __RPL__rpl_array_internal__h__

#include "rpl_array.h"

#include "rpl_adjbuffer.h"
#include "rpl_integer.h"
#include "rpl_value.h"


RPL_HEADER_BEGIN


struct rpl_array {
    rpl_adjbuffer_t _buffer;
    rpl_type_t _type;
};
typedef struct rpl_array rpl_array_t;

RPL_EXPORT
void
rpl_array_free(rpl_value_t array);

RPL_EXPORT
rpl_integer_t
rpl_array_offset_for_indices(rpl_value_t array, rpl_integer_t *indices);

RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_array_copy(rpl_value_t array);

RPL_EXPORT
const char * RPL_NULLABLE
rpl_array_copy_string(rpl_value_t array);


RPL_HEADER_END


#endif /* __RPL__rpl_array_internal__h__ */
