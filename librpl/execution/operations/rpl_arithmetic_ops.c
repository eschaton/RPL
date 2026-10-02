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


/* MARK: - Arithmetic Operations */

bool
rpl_arithmetic_op_using_table(rpl_context_t context,
			      rpl_typedispatch_entry_t *table)
{
    assert(context != NULL);
    assert(table != NULL);

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
rpl_arithmetic_op_$PLUS(rpl_operation_t operation,
			rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

    static rpl_typedispatch_entry_t PLUS_table[] = {
	{ rpl_type_real, rpl_type_real, rpl_real_plus_real },
//	{ rpl_type_real, rpl_type_complex, rpl_real_plus_complex },
//	{ rpl_type_complex, rpl_type_real, rpl_complex_plus_real },
//	{ rpl_type_complex, rpl_type_complex, rpl_complex_plus_complex},
//	{ rpl_type_array, rpl_type_array, rpl_array_plus_array },
//	{ rpl_type_real, rpl_type_symbol, rpl_real_plus_symbol },
//	{ rpl_type_symbol, rpl_type_real, rpl_symbol_plus_real },
//	{ rpl_type_symbol, rpl_type_symbol, rpl_symbol_plus_symbol },
//	{ rpl_type_list, rpl_type_list, rpl_list_plus_list },
//	{ rpl_type_list, RPL_TYPE_OBJ, rpl_list_plus_obj },
//	{ RPL_TYPE_OBJ, rpl_type_list, rpl_obj_plus_list },
//	{ rpl_type_string, rpl_type_string, rpl_string_plus_string },
//	{ RPL_TYPE_OBJ, rpl_type_string, rpl_obj_plus_string },
//	{ rpl_type_string, RPL_TYPE_OBJ, rpl_string_plus_obj },
	{ rpl_type_integer, rpl_type_real, rpl_integer_plus_real },
	{ rpl_type_real, rpl_type_integer, rpl_real_plus_real },
	{ rpl_type_integer, rpl_type_integer, rpl_integer_plus_integer},
//	{ rpl_type_unit, rpl_type_unit, rpl_unit_plus_unit },
//	{ rpl_type_symbol, rpl_type_unit, rpl_symbol_plus_unit },
//	{ rpl_type_unit, rpl_type_symbol, rpl_unit_plus_symbol },
	{ 0, 0, NULL},
    };

    return rpl_arithmetic_op_using_table(context, PLUS_table);
}


bool
rpl_arithmetic_op_$MINUS(rpl_operation_t operation,
			 rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

    static rpl_typedispatch_entry_t MINUS_table[] = {
	{ rpl_type_real, rpl_type_real, rpl_real_minus_real },
//	{ rpl_type_real, rpl_type_complex, rpl_real_minus_complex },
//	{ rpl_type_complex, rpl_type_real, rpl_complex_minus_real },
//	{ rpl_type_complex, rpl_type_complex,rpl_complex_minus_complex},
//	{ rpl_type_array, rpl_type_array, rpl_array_minus_array },
//	{ rpl_type_real, rpl_type_symbol, rpl_real_minus_symbol },
//	{ rpl_type_symbol, rpl_type_real, rpl_symbol_minus_real },
//	{ rpl_type_symbol, rpl_type_symbol, rpl_symbol_minus_symbol },
	{ rpl_type_integer, rpl_type_real, rpl_integer_minus_real },
	{ rpl_type_real, rpl_type_integer, rpl_real_minus_real },
	{ rpl_type_integer, rpl_type_integer,rpl_integer_minus_integer},
//	{ rpl_type_unit, rpl_type_unit, rpl_unit_minus_unit },
//	{ rpl_type_symbol, rpl_type_unit, rpl_symbol_minus_unit },
//	{ rpl_type_unit, rpl_type_symbol, rpl_unit_minus_symbol },
	{ 0, 0, NULL},
    };

    return rpl_arithmetic_op_using_table(context, MINUS_table);
}


bool
rpl_arithmetic_op_$TIMES(rpl_operation_t operation,
			 rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

    static rpl_typedispatch_entry_t TIMES_table[] = {
	{ rpl_type_real, rpl_type_real, rpl_real_times_real },
//	{ rpl_type_real, rpl_type_complex, rpl_real_times_complex },
//	{ rpl_type_complex, rpl_type_real, rpl_complex_times_real },
//	{ rpl_type_complex, rpl_type_complex,rpl_complex_times_complex},
//	{ rpl_type_array, rpl_type_real, rpl_array_times_real },
//	{ rpl_type_real, rpl_type_array, rpl_real_times_array },
//	{ rpl_type_array, rpl_type_array, rpl_array_times_array },
//	{ rpl_type_real, rpl_type_symbol, rpl_real_times_symbol },
//	{ rpl_type_symbol, rpl_type_real, rpl_symbol_times_real },
//	{ rpl_type_symbol, rpl_type_symbol, rpl_symbol_times_symbol },
	{ rpl_type_integer, rpl_type_real, rpl_integer_times_real },
	{ rpl_type_real, rpl_type_integer, rpl_real_times_real },
	{ rpl_type_integer, rpl_type_integer,rpl_integer_times_integer},
//	{ rpl_type_unit, rpl_type_unit, rpl_unit_times_unit },
//	{ rpl_type_symbol, rpl_type_unit, rpl_symbol_times_unit },
//	{ rpl_type_unit, rpl_type_symbol, rpl_unit_times_symbol },
	{ 0, 0, NULL},
    };

    return rpl_arithmetic_op_using_table(context, TIMES_table);
}


bool
rpl_arithmetic_op_$DIVIDE(rpl_operation_t operation,
			  rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

    static rpl_typedispatch_entry_t DIVIDE_table[] = {
	{ rpl_type_real, rpl_type_real, rpl_real_divide_real },
//	{ rpl_type_real, rpl_type_complex, rpl_real_divide_complex },
//	{ rpl_type_complex, rpl_type_real, rpl_complex_divide_real },
//	{ rpl_type_complex,rpl_type_complex,rpl_complex_divide_complex},
//	{ rpl_type_array, rpl_type_array, rpl_array_divide_array },
//	{ rpl_type_array, rpl_type_real, rpl_array_divide_real },
//	{ rpl_type_real, rpl_type_symbol, rpl_real_divide_symbol },
//	{ rpl_type_symbol, rpl_type_real, rpl_symbol_divide_real },
//	{ rpl_type_symbol, rpl_type_symbol, rpl_symbol_divide_symbol },
	{ rpl_type_integer, rpl_type_real, rpl_integer_divide_real },
	{ rpl_type_real, rpl_type_integer, rpl_real_divide_real },
	{ rpl_type_integer,rpl_type_integer,rpl_integer_divide_integer},
//	{ rpl_type_unit, rpl_type_unit, rpl_unit_divide_unit },
//	{ rpl_type_symbol, rpl_type_unit, rpl_symbol_divide_unit },
//	{ rpl_type_unit, rpl_type_symbol, rpl_unit_divide_symbol },
	{ 0, 0, NULL},
    };

    return rpl_arithmetic_op_using_table(context, DIVIDE_table);
}


RPL_SOURCE_END
