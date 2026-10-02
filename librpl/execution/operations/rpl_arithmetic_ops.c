//
//  rpl_arithmetic_ops.c
//  librpl
//
//  Created by Chris Hanson on 9/30/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//


#include "rpl_arithmetic_ops_internal.h"

#include <assert.h>

#include "rpl_integer_internal.h"
#include "rpl_real_internal.h"


RPL_SOURCE_BEGIN


rpl_op_definition_t rpl_arithmetic_op_defs[] = {
    { "+", rpl_operation_type_function, rpl_arithmetic_op_$PLUS },
    { "-", rpl_operation_type_function, rpl_arithmetic_op_$MINUS },
    { "*", rpl_operation_type_function, rpl_arithmetic_op_$TIMES },
    { "/", rpl_operation_type_function, rpl_arithmetic_op_$DIVIDE },
    { NULL, 0, NULL },
};


bool
rpl_configure_arithmetic_ops(rpl_operation_table_t table)
{
    assert(table != NULL);

    return rpl_operations_register_defs(table, rpl_arithmetic_op_defs);
}


/* MARK: - Type-Based Dispatch */

typedef rpl_value_t RPL_NULLABLE
	(*rpl_function_f)(rpl_value_t v1, rpl_value_t v2);

typedef struct rpl_typedispatch_entry {
    rpl_type_t type1;
    rpl_type_t type2;
    rpl_function_f RPL_NULLABLE function;
} rpl_typedispatch_entry_t;

/*! A wildcard that means "any type of object." */
#define RPL_TYPE_OBJ ((rpl_type_t)-1)

/*! Find an `rpl_function_f` in a table. */
rpl_function_f RPL_NULLABLE
rpl_function_find(rpl_typedispatch_entry_t *table, rpl_type_t t1,
		  rpl_type_t t2)
{
    rpl_function_f func = NULL;

    for (rpl_typedispatch_entry_t *entry = &table[0];
	 (entry->function != NULL) && (func == NULL);
	 entry++)
    {
	/* RPL_TYPE_OBJ is a wildcard */

	if (((entry->type1 == t1) && (entry->type2 == t2))
	    || ((entry->type1 == RPL_TYPE_OBJ) && (entry->type2 == t2))
	    || ((entry->type1 == t1) && (entry->type2 == RPL_TYPE_OBJ)))
	{
	    func = entry->function;
	}
    }

    return func;
}

/*!
 Call an `rpl_function_f`.

 We need a transparent placeholder to call an `rpl_function_f` because
 the type definition itself can't have the `RPL_RETURNS_RETAINED`
 annotation.
 */
rpl_value_t RPL_NULLABLE
rpl_function_call(rpl_function_f func, rpl_value_t v1, rpl_value_t v2)
RPL_RETURNS_RETAINED
{
    return (*func)(v1, v2);
}


/* MARK: - Arithmetic Operations */

bool
rpl_arithmetic_op_$PLUS(rpl_operation_t operation,
			rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

    rpl_stack_t stack = rpl_context_get_stack(context);
    assert(stack != NULL);

    if (rpl_stack_get_level(stack) < 2) {
	// TODO: Signal 'insufficient arguments' condition
	goto error;
    }

    rpl_value_t v2 = rpl_stack_pop(stack);
    assert(v2);

    rpl_value_t v1 = rpl_stack_pop(stack);
    assert(v1);

    rpl_type_t t1 = rpl_value_get_type(v1);
    rpl_type_t t2 = rpl_value_get_type(v2);

    static rpl_typedispatch_entry_t table[] = {
	{ rpl_type_real, rpl_type_real, rpl_real_add_real },
//	{ rpl_type_real, rpl_type_complex, rpl_real_add_complex },
//	{ rpl_type_complex, rpl_type_real, rpl_complex_add_real },
//	{ rpl_type_complex, rpl_type_complex, rpl_complex_add_complex },
//	{ rpl_type_array, rpl_type_array, rpl_array_add_array },
//	{ rpl_type_real, rpl_type_symbol, rpl_real_add_symbol },
//	{ rpl_type_symbol, rpl_type_real, rpl_symbol_add_real },
//	{ rpl_type_symbol, rpl_type_symbol, rpl_symbol_add_symbol },
//	{ rpl_type_list, rpl_type_list, rpl_list_add_list },
//	{ rpl_type_list, RPL_TYPE_OBJ, rpl_list_add_obj },
//	{ RPL_TYPE_OBJ, rpl_type_list, rpl_obj_add_list },
//	{ rpl_type_string, rpl_type_string, rpl_string_add_string },
//	{ RPL_TYPE_OBJ, rpl_type_string, rpl_obj_add_string },
//	{ rpl_type_string, RPL_TYPE_OBJ, rpl_string_add_obj },
	{ rpl_type_integer, rpl_type_real, rpl_integer_add_real },
	{ rpl_type_real, rpl_type_integer, rpl_real_add_real },
	{ rpl_type_integer, rpl_type_integer, rpl_integer_add_integer },
//	{ rpl_type_unit, rpl_type_unit, rpl_unit_add_unit },
//	{ rpl_type_symbol, rpl_type_unit, rpl_symbol_add_unit },
//	{ rpl_type_unit, rpl_type_symbol, rpl_unit_add_symbol },
	{ 0, 0, NULL},
    };

    rpl_function_f func = rpl_function_find(table, t1, t2);
    if (func) {
	rpl_value_t result = rpl_function_call(func, v1, v2);
	// TODO: Signal 'storage full' condition
	if (result == NULL) goto error;

	rpl_stack_push(stack, result);
	rpl_value_release(result);
    } else {
	// TODO: Signal 'type mismatch' condition
	goto error;
    }

    return true;

error:
    return false;
}


bool
rpl_arithmetic_op_$MINUS(rpl_operation_t operation,
			 rpl_context_t context)
{
    // TODO: Implement $MINUS
    return false;
}


bool
rpl_arithmetic_op_$TIMES(rpl_operation_t operation,
			 rpl_context_t context)
{
    // TODO: Implement $TIMES
    return false;
}


bool
rpl_arithmetic_op_$DIVIDE(rpl_operation_t operation,
			  rpl_context_t context)
{
    // TODO: Implement $DIVIDE
    return false;
}


RPL_SOURCE_END
