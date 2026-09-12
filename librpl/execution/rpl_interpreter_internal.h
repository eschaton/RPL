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


bool
rpl_interpreter_eval(rpl_interpreter_t interp, rpl_token_t token);


RPL_HEADER_END


#endif /* __RPL__rpl_interpreter_internal__h__ */
