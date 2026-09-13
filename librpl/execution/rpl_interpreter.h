//
//  rpl_interpreter.h
//  librpl
//
//  Created by Chris Hanson on 9/10/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_interpreter__h__
#define __RPL__rpl_interpreter__h__

#include "rpl_defines.h"

#include <stdbool.h>

#include "rpl_context.h"
#include "rpl_operation_table.h"
#include "rpl_unistring.h"


RPL_HEADER_BEGIN


/*! An instance of the RPL interpreter. */
typedef struct rpl_interpreter *rpl_interpreter_t;


/*!
 A function that configures the interpreter.

 The interpreter's context can be modified as desired by this function.
 Its main purpose is to populate the constant scope and operation table.



 @returns `true` on success, `false` on failure
 */
typedef bool (*rpl_configure_interpreter_f)(rpl_interpreter_t interp,
					    void * RPL_NULLABLE refcon);


/*!
 Creates and configures a new interpreter.

 Passing `NULL` for @a config uses the default configuration function,
 or the default configuration function can be wrapped in another
 function that further modifies the interpreter after its creation.
 */
RPL_EXPORT
rpl_interpreter_t RPL_NULLABLE
rpl_interpreter_new(rpl_configure_interpreter_f RPL_NULLABLE config,
		    void * RPL_NULLABLE refcon);

/*!
 An interpreter configuration function that uses all default values.

 It can be passed to ``rpl_interpreter_new`` with any @a refcon, or
 called from a wrapping interpreter configuration function with any
 @a refcon.
 */
RPL_EXPORT
bool
rpl_configure_defaults(rpl_interpreter_t interp,
		       void * RPL_NULLABLE refcon);

RPL_EXPORT
void
rpl_interpreter_free(rpl_interpreter_t interp);

RPL_EXPORT
rpl_context_t
rpl_interpreter_get_context(rpl_interpreter_t interp);

RPL_EXPORT
rpl_operation_table_t
rpl_interpreter_get_operation_table(rpl_interpreter_t interp);

RPL_EXPORT
bool
rpl_interpreter_append_input(rpl_interpreter_t interp,
			     rpl_unistring_t str);

RPL_EXPORT
bool
rpl_interpreter_has_output(rpl_interpreter_t interp);

RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_interpreter_copy_output(rpl_interpreter_t interp);

/*! Perform one iteration of the interpreter's read-eval-print loop. */
RPL_EXPORT
bool
rpl_interpreter_step(rpl_interpreter_t interp);

/*!
 Generate textual output representing the current state of the stack.

 This will look like

      2: 'foo'
      1: 'bar'
      0: 123

 for a three-level stack.
 */
RPL_EXPORT
bool
rpl_interpreter_output_stack(rpl_interpreter_t interp);


RPL_HEADER_END


#endif /* __RPL__rpl_interpreter__h__ */
