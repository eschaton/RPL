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

/*!
 An operation table iteration function.

 The function must not mutate the table or the operation.

 @returns `true` to continue iterating, `false` to stop
 */
typedef bool
(*rpl_operation_table_iter_f)(rpl_operation_table_t table,
			      rpl_operation_t op,
			      size_t idx,
			      void * RPL_NULLABLE refcon);

/*!
 Iterate an operation table.

 Calls a function once for each operation in the table.

 This **must not** be used to mutate the table; its main purpose is to
 allow something that presents a UI atop librpl to learn what operations
 are available.

 @returns `true` if iteration completed, `false` if it was incomplete
 */
RPL_EXPORT
bool
rpl_operation_table_iterate(rpl_operation_table_t table,
			    rpl_operation_table_iter_f iter,
			    void * RPL_NULLABLE refcon);


RPL_HEADER_END


#endif /* __RPL__rpl_operation_table__h__ */
