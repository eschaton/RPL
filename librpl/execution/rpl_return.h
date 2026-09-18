//
//  rpl_return.h
//  librpl
//
//  Created by Chris Hanson on 9/18/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_return__h__
#define __RPL__rpl_return__h__

#include "rpl_defines.h"

#include <stdbool.h>

#include "rpl_context.h"
#include "rpl_integer.h"


RPL_HEADER_BEGIN


/*!
 A "return" defers an operation's execution until parsing is complete.

 When implementing control flow, the first operation in a control-flow
 sequence creates an "open return" that completes the operation on the
 return stack. Subsequent operations in the control-flow sequence will
 look back through the return stack to the nearest "open" return and add
 themselves and the current stack level to it, or "close" the return and
 execute it using all of the information gathered so far.

 Take the following example, with empty value and return stacks:

      IF a b =
	THEN
	  1
	ELSE
	  0
      END

 The `IF` operation creates an open return on the return stack with
 `(IF_ret,0)` as its operation, where  `IF_ret` is the name of the
 operation that completes the `IF` upon being closed and 0 is the stack
 level to begin execution at upon closing.

 Since there is now a return on the return stack, the interpreter
 switches to deferred mode, and the *names* `'a'`, `'b'`, and `'='` are
 pushed on the value stack at levels 0-2.

 As with `IF`, `THEN` is an operation marked as "always immediate," so
 it's executed. It looks back through the return stack to the first open
 return, and adds `(THEN_ret,3)` to it. After this, `1` is pushed at
 level 3, `(ELSE_ret,4)` is added to the open return, `0` is pushed on
 the value stack at level 4, and `END` is immediately evaluated which
 closes the return.

 Once the return is closed, it can be invoked, which will cause
 `IF_ret` to be executed using the information added to it by the
 previous sequence and the passed context.
 */
typedef struct rpl_return *rpl_return_t;


/*!
 A return operation implementation.

 It takes the return as a parameter, as well as the context in which to
 perform that operation. A return operation is expected to express any
 error state via the passed context, but should also return `true` or
 `false` to indicate overall success or failure.
 */
typedef bool (*rpl_return_operation_t)(rpl_return_t ret,
				       rpl_context_t context);


RPL_EXPORT
rpl_return_t RPL_NULLABLE
rpl_return_new(rpl_return_operation_t operation,
	       rpl_integer_t level);

RPL_EXPORT
void
rpl_return_free(rpl_return_t ret);

RPL_EXPORT
bool
rpl_return_is_open(rpl_return_t ret);

RPL_EXPORT
rpl_integer_t
rpl_return_get_count(rpl_return_t ret);

RPL_EXPORT
rpl_return_operation_t
rpl_return_get_entry(rpl_return_t ret, rpl_integer_t idx,
		     rpl_integer_t *level);

RPL_EXPORT
bool
rpl_return_append(rpl_return_t ret,
		  rpl_return_operation_t operation,
		  rpl_integer_t level);

RPL_EXPORT
void
rpl_return_close(rpl_return_t ret);

RPL_EXPORT
bool
rpl_return_invoke(rpl_return_t ret, rpl_context_t context);


RPL_HEADER_END


#endif /* __RPL__rpl_return__h__ */

