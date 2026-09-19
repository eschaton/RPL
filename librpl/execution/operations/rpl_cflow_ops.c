//
//  rpl_cflow_ops.c
//  RPL
//
//  Created by Chris Hanson on 9/18/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_cflow_ops_internal.h"

#include <assert.h>

#include "rpl_operations_internal.h"
#include "rpl_return.h"


RPL_SOURCE_BEGIN


rpl_op_definition_t rpl_cflow_op_defs[] = {
    { "IF",	rpl_operation_type_immediate, rpl_cflow_op_IF	  },
    { "THEN",	rpl_operation_type_immediate, rpl_cflow_op_THEN   },
    { "ELSE",	rpl_operation_type_immediate, rpl_cflow_op_ELSE	  },
    { "DO",	rpl_operation_type_immediate, rpl_cflow_op_DO	  },
    { "UNTIL",	rpl_operation_type_immediate, rpl_cflow_op_UNTIL  },
    { "WHILE",	rpl_operation_type_immediate, rpl_cflow_op_WHILE  },
    { "REPEAT",	rpl_operation_type_immediate, rpl_cflow_op_REPEAT },
    { "END",	rpl_operation_type_immediate, rpl_cflow_op_END	  },
    { "FOR",	rpl_operation_type_immediate, rpl_cflow_op_FOR	  },
    { "START",	rpl_operation_type_immediate, rpl_cflow_op_START  },
    { "NEXT",	rpl_operation_type_immediate, rpl_cflow_op_NEXT	  },
    { "STEP",	rpl_operation_type_immediate, rpl_cflow_op_STEP	  },
    { NULL, 0, NULL },
};


bool
rpl_configure_cflow_ops(rpl_operation_table_t table)
{
    assert(table != NULL);

    return rpl_operations_register_defs(table, rpl_cflow_op_defs);
}


/* MARK: - Control Flow Operations */

/*!
 Push a return operation on the return stack.
 */
bool
rpl_cflow_push_ret_op(rpl_return_operation_t ret_op,
		      rpl_context_t context)
{
    assert(ret_op != NULL);
    assert(context != NULL);

    rpl_stack_t stack = rpl_context_get_stack(context);
    assert(stack != NULL);

    rpl_return_stack_t rstack = rpl_context_get_return_stack(context);
    assert(rstack != NULL);

    rpl_integer_t level = rpl_stack_get_level(stack);
    rpl_return_t ret = rpl_return_new(ret_op, level);
    if (ret == NULL) goto error;

    rpl_return_stack_push(rstack, ret);

    return true;

error:
    return false;
}

/*!
 Append an additional return operation to the most recent return
 operation on the return stack.
 */
bool
rpl_cflow_append_ret_op(rpl_return_operation_t ret_op,
			rpl_context_t context)
{
    assert(ret_op != NULL);
    assert(context != NULL);

    rpl_stack_t stack = rpl_context_get_stack(context);
    assert(stack != NULL);

    rpl_return_stack_t rstack = rpl_context_get_return_stack(context);
    assert(rstack != NULL);

    rpl_integer_t level = rpl_stack_get_level(stack);

    rpl_return_t ret = rpl_return_stack_get_top_open(rstack);
    /* TODO: Signal 'no open return operation' condition */
    if (ret == NULL) goto error;

    bool appended = rpl_return_append(ret, ret_op, level);
    /* TODO: Signal 'resources exhausted' condition */
    if (appended == false) goto error;

    return true;

error:
    return false;
}

#define RPL_CFLOW_OP_DEFINE(name,pora) \
    bool \
    rpl_cflow_op_ ## name (rpl_operation_t operation, \
			   rpl_context_t context) \
    { \
	assert(operation != NULL); \
	assert(context != NULL); \
	return rpl_cflow_ ## pora ## _ret_op( \
		     rpl_cflow_op_ ## name ## _ret, \
		     context); \
    } \

RPL_CFLOW_OP_DEFINE(IF, push)
RPL_CFLOW_OP_DEFINE(THEN, append)
RPL_CFLOW_OP_DEFINE(ELSE, append)
RPL_CFLOW_OP_DEFINE(DO, push)
RPL_CFLOW_OP_DEFINE(WHILE, append)
RPL_CFLOW_OP_DEFINE(REPEAT, push)
RPL_CFLOW_OP_DEFINE(UNTIL, append)

bool
rpl_cflow_op_END(rpl_operation_t operation,
		 rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

    /* Get the topmost open return on the return stack and close it. */

    rpl_return_stack_t rstack = rpl_context_get_return_stack(context);
    assert(rstack != NULL);

    rpl_return_t ret = rpl_return_stack_get_top_open(rstack);
    if (ret == NULL) goto error;

    rpl_integer_t level;
    rpl_return_operation_t ret_op = rpl_return_get_entry(ret, 0,
							 &level);
    assert(ret_op != NULL);

    /* Ensure the right operation is being closed. */
    if ((ret_op == rpl_cflow_op_IF_ret)
	|| (ret_op == rpl_cflow_op_DO_ret)
	|| (ret_op == rpl_cflow_op_REPEAT_ret))
    {
	rpl_return_close(ret);
    } else {
	// TODO: Signal 'invalid operation' condition
	goto error;
    }

    return true;

error:
    return false;
}

RPL_CFLOW_OP_DEFINE(FOR, push)
RPL_CFLOW_OP_DEFINE(START, push)

bool
rpl_cflow_op_NEXT(rpl_operation_t operation,
		  rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

    /* Get the topmost open return on the return stack and close it. */

    rpl_return_stack_t rstack = rpl_context_get_return_stack(context);
    assert(rstack != NULL);

    rpl_return_t ret = rpl_return_stack_get_top_open(rstack);
    if (ret == NULL) goto error;

    rpl_integer_t level;
    rpl_return_operation_t ret_op = rpl_return_get_entry(ret, 0,
							 &level);
    assert(ret_op != NULL);

    /* Ensure the right operation is being closed. */
    if ((ret_op == rpl_cflow_op_FOR_ret)
	|| (ret_op == rpl_cflow_op_START_ret))
    {
	rpl_return_close(ret);
    } else {
	// TODO: Signal 'invalid operation' condition
	goto error;
    }

    return true;

error:
    return false;
}

bool
rpl_cflow_op_STEP(rpl_operation_t operation,
		  rpl_context_t context)
{
    assert(operation != NULL);
    assert(context != NULL);

    /* Get the topmost open return on the return stack and close it. */

    rpl_return_stack_t rstack = rpl_context_get_return_stack(context);
    assert(rstack != NULL);

    rpl_return_t ret = rpl_return_stack_get_top_open(rstack);
    if (ret == NULL) goto error;

    rpl_integer_t level;
    rpl_return_operation_t ret_op = rpl_return_get_entry(ret, 0,
							 &level);
    assert(ret_op != NULL);

    /* Ensure the right operation is being closed. */
    if ((ret_op == rpl_cflow_op_FOR_ret)
	|| (ret_op == rpl_cflow_op_START_ret))
    {
	rpl_return_close(ret);
    } else {
	// TODO: Signal 'invalid operation' condition
	goto error;
    }

    return true;

error:
    return false;
}


/* MARK: - Control Flow Return Operations */

bool
rpl_cflow_op_IF_ret(rpl_return_t ret,
		    rpl_context_t context)
{
    assert(ret != NULL);
    assert(context != NULL);

    return false;//xxx
}

bool
rpl_cflow_op_THEN_ret(rpl_return_t ret,
		      rpl_context_t context)
{
    assert(ret != NULL);
    assert(context != NULL);

    return false;//xxx
}

bool rpl_cflow_op_ELSE_ret(rpl_return_t ret,
			   rpl_context_t context)
{
    assert(ret != NULL);
    assert(context != NULL);

    return false;//xxx
}

bool rpl_cflow_op_DO_ret(rpl_return_t ret,
			 rpl_context_t context)
{
    assert(ret != NULL);
    assert(context != NULL);

    return false;//xxx
}

bool rpl_cflow_op_WHILE_ret(rpl_return_t ret,
			    rpl_context_t context)
{
    assert(ret != NULL);
    assert(context != NULL);

    return false;//xxx
}

bool rpl_cflow_op_REPEAT_ret(rpl_return_t ret,
			     rpl_context_t context)
{
    assert(ret != NULL);
    assert(context != NULL);

    return false;//xxx
}

bool rpl_cflow_op_UNTIL_ret(rpl_return_t ret,
			   rpl_context_t context)
{
    assert(ret != NULL);
    assert(context != NULL);

    return false;//xxx
}

bool rpl_cflow_op_FOR_ret(rpl_return_t ret,
			  rpl_context_t context)
{
    assert(ret != NULL);
    assert(context != NULL);

    return false;//xxx
}

bool rpl_cflow_op_START_ret(rpl_return_t ret,
			    rpl_context_t context)
{
    assert(ret != NULL);
    assert(context != NULL);

    return false;//xxx
}


RPL_SOURCE_END
