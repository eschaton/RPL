//
//  rpl_condition_internal.h
//  librpl
//
//  Created by Chris Hanson on 10/9/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_condition.h"

#include <unistd.h>

#include "rpl_scope.h"


#ifndef __RPL__rpl_condition_internal__h__
#define __RPL__rpl_condition_internal__h__


RPL_HEADER_BEGIN


typedef struct rpl_condition {
    rpl_unistring_t _name;
    rpl_scope_t _payload;
} rpl_condition_t;


RPL_EXPORT
void
rpl_condition_free(rpl_value_t condition);

RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_condition_copy_string(rpl_value_t condition, rpl_environment_t env);

/*! Initialize the condition system and standard conditions. */
RPL_EXPORT
bool
rpl_condition_initialize(void);


/* MARK: - Signaling & Handling Conditions */

/*!
 A handler for when a condition is raised.

 The handler is passed the condition itself as well as its index within
 the condition stack and the context passed when it was registered.

 This is primarily for the interpreter's use.
 */
typedef void (*rpl_condition_handler_f)(rpl_value_t condition,
					size_t idx,
					void * RPL_NULLABLE context);

/*! Install a condition handler. */
RPL_EXPORT
void
rpl_condition_handler_install(rpl_condition_handler_f handler,
			      void * RPL_NULLABLE context);

/*! Get the number of conditions on the condition stack. */
RPL_EXPORT
size_t
rpl_condition_stack_get_count(void);

/*! Get the condition at the given level of the condition stack. */
RPL_EXPORT
rpl_value_t
rpl_condition_stack_get(size_t level);

/*! Reset the condition stack after all handling is complete. */
RPL_EXPORT
bool
rpl_condition_stack_reset(void);


RPL_HEADER_END


#endif /* __RPL__rpl_condition_internal__h__ */
