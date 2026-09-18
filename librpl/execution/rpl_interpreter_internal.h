//
//  rpl_interpreter_internal.h
//  librpl
//
//  Created by Chris Hanson on 9/10/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_interpreter_internal__h__
#define __RPL__rpl_interpreter_internal__h__

#include "rpl_interpreter.h"

#include "rpl_context.h"
#include "rpl_operation.h"
#include "rpl_operation_table.h"
#include "rpl_scope.h"
#include "rpl_tokenizer.h"


RPL_HEADER_BEGIN


struct rpl_interpreter {
    rpl_context_t _context;
    rpl_tokenizer_t _tokenizer;
    rpl_operation_table_t _optable;
    rpl_unistring_t _output;
};


/*!
 The evaluation modes the interpreter supports.
*/
typedef enum rpl_interpreter_mode {
    /*!
     In immediate mode, identifiers corresponding to commands and
     variables are evaluated by the interpreter as soon as they are
     encountered.
     */
    rpl_interpreter_mode_immedate = 0,

    /*!
     In deferred mode, identifiers corresponding to commands and
     variables push a name on the stack, on the assumption that they
     will be evaluated separately later.
     */
    rpl_interpreter_mode_deferred,
} rpl_interpreter_mode_t;


/*!
 Evaluate one token in the interpreter's current context.

 Evaluates a token according to the @a mode.
 */
RPL_EXPORT
bool
rpl_interpreter_eval_token(rpl_interpreter_t interp, rpl_token_t token,
			   rpl_interpreter_mode_t mode);


RPL_HEADER_END


#endif /* __RPL__rpl_interpreter_internal__h__ */
