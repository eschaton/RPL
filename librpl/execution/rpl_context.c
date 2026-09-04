//
//  rpl_context.c
//  RPL
//
//  Created by Chris Hanson on 9/3/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_context_internal.h"

#include <assert.h>
#include <stdlib.h>


RPL_SOURCE_BEGIN


rpl_context_t RPL_NULLABLE
rpl_context_new(void)
{
    rpl_context_t context = calloc(1, sizeof(struct rpl_context));
    if (context) {
	context->_environment = rpl_environment_new();
	if (context->_environment == NULL) goto error;

	context->_stack = rpl_stack_new(1024);
	if (context->_stack == NULL) goto error;
    }
    return context;

error:
    rpl_context_free(context);
    return NULL;
}

void
rpl_context_free(rpl_context_t context)
{
    assert(context != NULL);

    if (context->_environment) {
	rpl_environment_free(context->_environment);
    }

    if (context->_stack) {
	rpl_stack_free(context->_stack);
    }

    free(context);
}

rpl_environment_t
rpl_context_get_environment(rpl_context_t context)
{
    assert(context != NULL);

    return context->_environment;
}

rpl_stack_t
rpl_context_get_stack(rpl_context_t context)
{
    assert(context != NULL);

    return context->_stack;
}


RPL_SOURCE_END
