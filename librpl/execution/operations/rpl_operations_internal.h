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


/* Macro for declaring an operation. */
#define RPL_OPERATION_DECLARE(kind,name) \
    bool rpl_ ## kind ## _op_ ## name (rpl_operation_t operation, \
				       rpl_context_t context)


/* MARK: - Type-Based Dispatch */

/*! A two-valued function that either returns a new value or `NULL`. */
typedef rpl_value_t RPL_NULLABLE
	(*rpl_function_f)(rpl_value_t v1, rpl_value_t v2);

/*! One entry in a type-dispatch function table. */
typedef struct rpl_typedispatch_entry {
    rpl_type_t type1;
    rpl_type_t type2;
    rpl_function_f RPL_NULLABLE function;
} rpl_typedispatch_entry_t;

/*! A wildcard that means "any type of object." */
#define RPL_TYPE_OBJ ((rpl_type_t)-1)

/*! Find an `rpl_function_f` in a type-dispatch table. */
rpl_function_f RPL_NULLABLE
rpl_function_find(rpl_typedispatch_entry_t *table, rpl_type_t t1,
		  rpl_type_t t2);

/*!
 Call an `rpl_function_f`.

 We need a transparent placeholder to call an `rpl_function_f` because
 the type definition itself can't have the `RPL_RETURNS_RETAINED`
 annotation.
 */
static inline __attribute__((always_inline))
rpl_value_t RPL_NULLABLE
rpl_function_call(rpl_function_f func, rpl_value_t v1, rpl_value_t v2)
RPL_RETURNS_RETAINED
{
    return (*func)(v1, v2);
}


RPL_HEADER_END


#endif /* __RPL__rpl_operations_internal__h__ */
