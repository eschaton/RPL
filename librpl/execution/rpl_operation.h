//
//  rpl_operation.h
//  RPL
//
//  Created by Chris Hanson on 9/3/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__librpl_operation__h__
#define __RPL__librpl_operation__h__

#include "rpl_defines.h"

#include <stdbool.h>

#include "rpl_context.h"


RPL_HEADER_BEGIN


/*! An operation is an action taken in RPL. */
typedef struct rpl_operation *rpl_operation_t;


/*! RPL supports several types of operations. */
typedef enum rpl_operation_type {
    /*!
     An immediate operation may only be performed directly on the
     command line, not used in a program.
     */
    rpl_operation_type_immediate = 0,

    /*!
     A command, sometimes called a procedure, is an operation that
     performs an action but does not return a value.
     */
    rpl_operation_type_command,

    /*! A function is an operation that returns a value. */
    rpl_operation_type_function,

    /*!
     An analytic function is an operation that returns a value and also
     has an inverse and a derivative.
     */
    rpl_operation_type_analytic_function,
} rpl_operation_type_t;


/*!
 An operation implementation.

 It takes the operation object itself as a parameter, as well as the
 context in which to performthat operation. The operation is expected to
 express any error state via the passed context, but should also return
 `true` or `false` to indicate overall success or failure.
 */
typedef bool (*rpl_operation_impl_t)(rpl_operation_t operation,
				     rpl_context_t context);

/*!
 Create a new operation.

 - NOTE: The operation makes a private copy of its name.
 */
RPL_EXPORT
rpl_operation_t RPL_NULLABLE
rpl_operation_new(const char *name,
		  rpl_operation_type_t type,
		  rpl_operation_impl_t impl,
		  void * RPL_NULLABLE refcon);

RPL_EXPORT
void
rpl_operation_free(rpl_operation_t op);

RPL_EXPORT
const char *
rpl_operation_get_name(rpl_operation_t op);

RPL_EXPORT
rpl_operation_type_t
rpl_operation_get_type(rpl_operation_t op);

RPL_EXPORT
void * RPL_NULLABLE
rpl_operation_get_refcon(rpl_operation_t op);

/*! Perform the operation in the given context. */
RPL_EXPORT
bool
rpl_operation_invoke(rpl_operation_t op, rpl_context_t context);


RPL_HEADER_END


#endif /* __RPL__librpl_operation__h__*/
