//
//  rpl_program.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_program__h__
#define __RPL__rpl_program__h__

#include "rpl_value.h"

#include "rpl_integer.h"


RPL_HEADER_BEGIN


/*!
 A program in RPL is a sequence of values (including identifiers) within
 `«` and `»` chevron delimiters that can be evaluated to perform some
 action.

 When a program is evaluated, its values are evaluated in sequence,
 according to the same evaluation rules as the immediate-mode evaluator:
 Values are pushed on the stack, identifiers that correspond to known
 operations cause those operations to be invoked, identifiers that
 correspond to known variables (at the time of evaluation) cause their
 values to be pushed on the stack, and unknown identifiers cause names
 to be pushed on the stack.
 */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_program_new(void);

/*! Get the number of values that make up the program. */
RPL_EXPORT
rpl_integer_t
rpl_program_get_count(rpl_value_t program);

/*! Get one of the values that makes up the program. */
RPL_EXPORT
rpl_value_t
rpl_program_get_value(rpl_value_t program, rpl_integer_t idx);

/*! Add a value to the program, at the end. */
RPL_EXPORT
bool
rpl_program_append(rpl_value_t program, rpl_value_t value);


RPL_HEADER_END


#endif /* __RPL__rpl_program__h__ */
