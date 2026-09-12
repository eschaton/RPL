//
//  rpl_operation.c
//  librpl
//
//  Created by Chris Hanson on 9/3/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_operation_internal.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


RPL_SOURCE_BEGIN


rpl_operation_t RPL_NULLABLE
rpl_operation_new(rpl_unistring_t name,
		  rpl_operation_type_t type,
		  rpl_operation_impl_t impl,
		  void * RPL_NULLABLE refcon)
{
    assert(name != NULL);
    assert(impl != NULL);
    
    rpl_operation_t op = calloc(1, sizeof(struct rpl_operation));
    if (op) {
	op->_name = rpl_unistring_copy(name);
	if (op->_name == NULL) goto error;
	op->_type = type;
	op->_impl = impl;
	op->_refcon = refcon;
    }
    return op;

error:
    rpl_operation_free(op);
    return NULL;
}

void
rpl_operation_free(rpl_operation_t op)
{
    assert(op != NULL);

    if (op->_name) rpl_unistring_release(op->_name);

    free(op);
}

rpl_unistring_t
rpl_operation_get_name(rpl_operation_t op)
{
    assert(op != NULL);

    return op->_name;
}

rpl_operation_type_t
rpl_operation_get_type(rpl_operation_t op)
{
    assert(op != NULL);

    return op->_type;
}

void * RPL_NULLABLE
rpl_operation_get_refcon(rpl_operation_t op)
{
    assert(op != NULL);

    return op->_refcon;
}

bool
rpl_operation_invoke(rpl_operation_t op, rpl_context_t context)
{
    assert(op != NULL);
    assert(context != NULL);

    return (*op->_impl)(op, context);
}


RPL_SOURCE_END
