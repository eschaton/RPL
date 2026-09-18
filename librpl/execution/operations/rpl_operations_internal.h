//
//  rpl_operations_internal.h
//  librpl
//
//  Created by Chris Hanson on 9/17/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_operations_internal__h__
#define __RPL__rpl_operations_internal__h__

#include "rpl_operations.h"


RPL_HEADER_BEGIN


/*!
 An operation definition used to register a collection of operation
 implementations with the operation table.
 */
typedef struct {
    const char * RPL_NULLABLE _name;
    rpl_operation_type_t _type;
    rpl_operation_impl_t _impl;
} rpl_op_definition_t;


RPL_EXPORT
bool
rpl_operations_register_defs(rpl_operation_table_t table,
			     rpl_op_definition_t *defs);


RPL_HEADER_END


#endif /* __RPL__rpl_operations_internal__h__ */
