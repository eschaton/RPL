//
//  rpl_logical_ops.c
//  librpl
//
//  Created by Chris Hanson on 9/19/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_logical_ops_internal.h"

#include <assert.h>

#include "rpl_operations_internal.h"
#include "rpl_real.h"


RPL_SOURCE_BEGIN


rpl_op_definition_t rpl_logical_op_defs[] = {
    { "<",	rpl_operation_type_function, rpl_logical_op_LT	     },
    { "≤",	rpl_operation_type_function, rpl_logical_op_LTE      },
    { ">",	rpl_operation_type_function, rpl_logical_op_GT	     },
    { "≥",	rpl_operation_type_function, rpl_logical_op_GTE	     },
    { "==",	rpl_operation_type_function, rpl_logical_op_EQUAL    },
    { "≠",	rpl_operation_type_function, rpl_logical_op_NOTEQUAL },
    { NULL, 0, NULL },
};


bool
rpl_configure_logical_ops(rpl_operation_table_t table)
{
    assert(table != NULL);

    return rpl_operations_register_defs(table, rpl_logical_op_defs);
}


/* MARK: - Logical Operations */

bool
rpl_logical_op_push_result(rpl_stack_t stack, bool result)
{
    assert(stack != NULL);

    //xxx

    return true;
}

bool
rpl_logical_op_LT(rpl_operation_t operation,
		  rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

    rpl_stack_t stack = rpl_context_get_stack(context);
    assert(stack != NULL);

    if (rpl_stack_get_level(stack) >= 2) {
	rpl_value_t y = rpl_stack_pop(stack);
	rpl_value_t x = rpl_stack_pop(stack);

	rpl_type_t yt = rpl_value_get_type(y);
	rpl_type_t xt = rpl_value_get_type(x);

	if (xt < yt) {
	    switch (xt) {
		case rpl_type_integer: {
		    rpl_real_t yr = rpl_real_get_rep(y);
		    rpl_real_t xr = rpl_real_get_rep(x);
		    bool did;
		    if (xr < yr) {
			did = rpl_logical_op_push_result(stack, true);
		    } else {
			did = rpl_logical_op_push_result(stack, false);
		    }
		    if (did == false) goto error;
		} break;

		case rpl_type_real: {
		    rpl_integer_t yr = rpl_integer_get_rep(y);
		    rpl_integer_t xr = rpl_integer_get_rep(x);
		    bool did;
		    if (xr < yr) {
			did = rpl_logical_op_push_result(stack, true);
		    } else {
			did = rpl_logical_op_push_result(stack, false);
		    }
		    if (did == false) goto error;
		} break;

		case rpl_type_string: {
		    // TODO: String <
		    bool did = rpl_logical_op_push_result(stack, false);
		    if (did == false) goto error;
		} break;

		case rpl_type_unit: {
		    // TODO: Unit <
		    bool did = rpl_logical_op_push_result(stack, false);
		    if (did == false) goto error;
		} break;

		default: {
		    // TODO: Signal 'invalid type' condition
		    goto error;
		} break;
	    }
	} else {
	    /*
	     TODO: Handle different type arguments.

	     The allowed combinations are:

	     x		'symb'	=> 'x<symb'
	     'symb'	x	=> 'symb<x'
	     x_unit	'symb'	=> 'x_unit<symb'
	     'symb'	x_unit	=> 'symb<x_unit'
	     */
	    bool did = rpl_logical_op_push_result(stack, false);
	    if (did == false) goto error;
	}
    } else {
	/* TODO: Signal 'insufficient arguments' condition */
	goto error;
    }

    return true;

error:
    return false;//xxx
}

bool
rpl_logical_op_LTE(rpl_operation_t operation,
		   rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

error:
    return false;//xxx
}

bool
rpl_logical_op_GT(rpl_operation_t operation,
		  rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

error:
    return false;//xxx
}

bool
rpl_logical_op_GTE(rpl_operation_t operation,
		   rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

error:
    return false;//xxx
}

bool
rpl_logical_op_EQUAL(rpl_operation_t operation,
		     rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

error:
    return false;//xxx
}

bool
rpl_logical_op_NOTEQUAL(rpl_operation_t operation,
			rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

error:
    return false;//xxx
}


RPL_SOURCE_END
