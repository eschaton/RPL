//
//  rpl_list_internal.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_list_internal__h__
#define __RPL__rpl_list_internal__h__

#include "rpl_list.h"

#include "rpl_value.h"


RPL_HEADER_BEGIN


struct rpl_list {
    /*! The first value in the pair. */
    rpl_value_t RPL_NULLABLE _car;

    /*! The second value in the pair. */
    rpl_value_t RPL_NULLABLE _cdr;
};
typedef struct rpl_list rpl_list_t;


RPL_EXPORT
void
rpl_list_free(rpl_value_t val);

RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_list_copy(rpl_value_t list);

RPL_EXPORT
const char * RPL_NULLABLE
rpl_list_copy_string(rpl_value_t list);


RPL_HEADER_END


#endif /* __RPL__rpl_list_internal__h__ */
