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

#include "rpl_unistring.h"


RPL_HEADER_BEGIN


/*! An instance of the RPL interpreter. */
typedef struct rpl_interpreter *rpl_interpreter_t;


RPL_EXPORT
rpl_interpreter_t RPL_NULLABLE
rpl_interpreter_new(void);

RPL_EXPORT
void
rpl_interpreter_free(rpl_interpreter_t interp);

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


RPL_HEADER_END


#endif /* __RPL__rpl_interpreter__h__ */
