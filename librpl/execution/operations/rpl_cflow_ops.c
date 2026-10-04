//
//  rpl_cflow_ops.c
//  librpl
//
//  Created by Chris Hanson on 9/30/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//


#include "rpl_cflow_ops_internal.h"

#include <assert.h>

#include "rpl_interpreter_internal.h"


RPL_SOURCE_BEGIN


rpl_op_definition_t rpl_cflow_op_defs[] = {
    {"IFT",        rpl_operation_type_command,rpl_cflow_op_IFT},
    {"IFTE",       rpl_operation_type_command,rpl_cflow_op_IFTE},
    {"_DO",        rpl_operation_type_command,rpl_cflow_op__DO},
    {"_FORNEXT",   rpl_operation_type_command,rpl_cflow_op__FORNEXT},
    {"_FORSTEP",   rpl_operation_type_command,rpl_cflow_op__FORSTEP},
    {"_CASE",      rpl_operation_type_command,rpl_cflow_op__CASE},
    {"_STARTNEXT", rpl_operation_type_command,rpl_cflow_op__STARTNEXT},
    {"_STARTSTEP", rpl_operation_type_command,rpl_cflow_op__STARTSTEP},
    {"_WHILE",     rpl_operation_type_command,rpl_cflow_op__WHILE},
    {NULL, 0, NULL },
};


bool
rpl_configure_cflow_ops(rpl_operation_table_t table)
{
    assert(table != NULL);

    return rpl_operations_register_defs(table, rpl_cflow_op_defs);
}


/* MARK: - Control Flow Operations */

bool
rpl_cflow_op_if_then_else(rpl_operation_t operation,
			  rpl_context_t context,
			  bool has_else)
{
    assert(operation != NULL);
    assert(context != NULL);

    rpl_stack_t stack = rpl_context_get_stack(context);
    assert(stack != NULL);

    rpl_interpreter_t interp = rpl_context_get_interpreter(context);
    assert(interp != NULL);

    rpl_value_t test_value = NULL;
    rpl_value_t true_value = NULL;
    rpl_value_t false_value = NULL;

    const rpl_integer_t expected_arguments = has_else ? 3 : 2;

    if (rpl_stack_get_level(stack) < expected_arguments) {
	// TODO: Signal 'insufficient arguments' condition
	goto error;
    }

    test_value = rpl_stack_pop(stack);
    assert(test_value != NULL);

    true_value = rpl_stack_pop(stack);
    assert(true_value != NULL);

    if (has_else) {
	false_value = rpl_stack_pop(stack);
	assert(false_value != NULL);
    }

    /*
     If the test is a program, it has to be evaluated to determine the
     truth value to use. Otherwise, it can just be coerced to truth.
     */

    bool test_result;
    rpl_type_t test_type = rpl_value_get_type(test_value);
    if (test_type == rpl_type_program) {
	bool evaluated = rpl_interpreter_eval_program(interp,
						      test_value);
	if (evaluated == false) goto error;

	rpl_value_t test_result_value = rpl_stack_pop(stack);
	assert(test_result_value != NULL);

	test_result = rpl_value_is_true(test_result_value);

	rpl_value_release(test_result_value);
    } else {
	test_result = rpl_value_is_true(test_value);
    }

    /*
     If the test result was true, handle the true clause. If the true
     clause is a program, it has to be evaluated, otherwise the true
     clause can just be pushed on the stack.

     The same is true for the false clause, if any.
     */

    if (test_result) {
	rpl_type_t true_type = rpl_value_get_type(true_value);
	if (true_type == rpl_type_program) {
	    bool evaluated = rpl_interpreter_eval_program(interp,
							  true_value);
	    if (evaluated == false) goto error;

	    /*
	     Unlike with the test clause, leave any values pushed on the
	     stack by the true clause in place; that's the desired
	     behavior of the construct.
	     */
	} else {
	    rpl_stack_push(stack, true_value);
	}
    } else if (!test_result && has_else) {
	rpl_type_t false_type = rpl_value_get_type(false_value);
	if (false_type == rpl_type_program) {
	    bool evaluated = rpl_interpreter_eval_program(interp,
							  false_value);
	    if (evaluated == false) goto error;

	    /*
	     Unlike with the test clause, leave any values pushed on the
	     stack by the false clause in place; that's the desired
	     behavior of the construct.
	     */
	} else {
	    rpl_stack_push(stack, true_value);
	}
    }

    rpl_value_release(test_value);
    rpl_value_release(true_value);
    if (has_else) {
	rpl_value_release(false_value);
    }

    return true;

error:
    if (test_value) rpl_value_release(test_value);
    if (true_value) rpl_value_release(true_value);
    if (false_value) rpl_value_release(false_value);
    return false;
}

bool
rpl_cflow_op_IFT(rpl_operation_t operation,
		 rpl_context_t context)
{
    return rpl_cflow_op_if_then_else(operation, context, false);
}

bool
rpl_cflow_op_IFTE(rpl_operation_t operation,
		  rpl_context_t context)
{
    return rpl_cflow_op_if_then_else(operation, context, true);
}

bool
rpl_cflow_op__DO(rpl_operation_t operation,
		 rpl_context_t context)
{
    // TODO: Implement iDO
    return false;
}

bool
rpl_cflow_op__FORNEXT(rpl_operation_t operation,
		      rpl_context_t context)
{
    // TODO: Implement iFORNEXT
    return false;
}

bool
rpl_cflow_op__FORSTEP(rpl_operation_t operation,
		      rpl_context_t context)
{
    // TODO: Implement iFORSTEP
    return false;
}

bool
rpl_cflow_op__CASE(rpl_operation_t operation,
		   rpl_context_t context)
{
    // TODO: Implement iCASE
    return false;
}

bool
rpl_cflow_op__STARTNEXT(rpl_operation_t operation,
			rpl_context_t context)
{
    // TODO: Implement iSTARTNEXT
    return false;
}

bool
rpl_cflow_op__STARTSTEP(rpl_operation_t operation,
			rpl_context_t context)
{
    // TODO: Implement iSTARTSTEP
    return false;
}

bool
rpl_cflow_op__WHILE(rpl_operation_t operation,
		    rpl_context_t context)
{
    // TODO: Implement iWHILE
    return false;
}


RPL_SOURCE_END
