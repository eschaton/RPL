//
//  rpl_context.h
//  librpl
//
//  Created by Chris Hanson on 9/3/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_context__h__
#define __RPL__rpl_context__h__

#include "rpl_defines.h"

#include "rpl_environment.h"
#include "rpl_return_stack.h"
#include "rpl_scope.h"
#include "rpl_stack.h"


RPL_HEADER_BEGIN


/*! An RPL execution context: Everything needed to execute RPL. */
typedef struct rpl_context *rpl_context_t;


RPL_EXPORT
rpl_context_t RPL_NULLABLE
rpl_context_new(void);

RPL_EXPORT
void
rpl_context_free(rpl_context_t context);

RPL_EXPORT
rpl_environment_t
rpl_context_get_environment(rpl_context_t context);

RPL_EXPORT
rpl_stack_t
rpl_context_get_stack(rpl_context_t context);

RPL_EXPORT
rpl_return_stack_t
rpl_context_get_return_stack(rpl_context_t context);

/*! Get the scope in which predefined constants exist. */
RPL_EXPORT
rpl_scope_t
rpl_context_get_constant_scope(rpl_context_t context);

/*! Get the scope in which global variables exist. */
RPL_EXPORT
rpl_scope_t
rpl_context_get_global_scope(rpl_context_t context);

/*! Get the scope in which local variables exist. */
RPL_EXPORT
rpl_scope_t
rpl_context_get_local_scope(rpl_context_t context);


RPL_HEADER_END


#endif /* __RPL__rpl_context__h__ */
