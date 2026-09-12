//
//  rpl_operation_table.h
//  librpl
//
//  Created by Chris Hanson on 9/11/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_operation_table__h__
#define __RPL__rpl_operation_table__h__

#include "rpl_defines.h"

#include "rpl_operation.h"
#include "rpl_unistring.h"


RPL_HEADER_BEGIN


/*! A table of known, named operations in RPL. */
typedef struct rpl_operation_table *rpl_operation_table_t;


RPL_EXPORT
rpl_operation_table_t RPL_NULLABLE
rpl_operation_table_new(void);

RPL_EXPORT
void
rpl_operation_table_free(rpl_operation_table_t table);

RPL_EXPORT
rpl_operation_t RPL_NULLABLE
rpl_operation_table_get(rpl_operation_table_t table,
			rpl_unistring_t name);

RPL_EXPORT
bool
rpl_operation_table_set(rpl_operation_table_t table,
			rpl_operation_t op);


RPL_HEADER_END


#endif /* __RPL__rpl_operation_table__h__ */
