//
//  rpl_tokenizer.h
//  RPL
//
//  Created by Chris Hanson on 9/3/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_tokenizer__h__
#define __RPL__rpl_tokenizer__h__

#include "rpl_defines.h"

#include "rpl_context.h"
#include "rpl_token.h"


RPL_HEADER_BEGIN


/*! The RPL tokenizer itself. */
typedef struct rpl_tokenizer *rpl_tokenizer_t;


RPL_EXPORT
rpl_tokenizer_t RPL_NULLABLE
rpl_tokenizer_new(rpl_context_t context);

RPL_EXPORT
void
rpl_tokenizer_free(rpl_tokenizer_t tokenizer);

/*!
 Append UTF-8 text to the tokenizer's buffer.

 - NOTE: Some form of whitespace is required after every token; this is
         most easily handled by appending a newline after other text if
	 one is not already present.
 */
RPL_EXPORT
bool
rpl_tokenizer_append(rpl_tokenizer_t tokenizer, const char *str);

/*!
 Copy the next token from the tokenizer.

 RPL syntax is straightforward:

     integer = '#' ' '? /[0-9A-Fa-f]+[bdho]?/.
     real = /[+-]?[0-9]*(.[0-9]*)?([eE][+-][0-9]+)?/.
     complex = '(' real ',' /[∡∢]?/ real ')'.
     array_element = real | complex | array.
     array = '[' array_element* ']'.
     name_text = character+.
     name = '\'' name_text '\''.
     program = '«' token+ '»'.
     string = '"' character* '"'.
     list = '{' value* '}'.
     tagged = ':' name_text ':' value.
     unit = real '_' name_text.
     value = integer | real | complex | array | name
           | program | string | list | tagged | unit.
     identifier = name_text.
     comment = '@' character* end_of_line.

 Almost all syntactic units in the grammar above are separable by
 whitespace other than the following:

 - binary integers allow (but don't require) exactly one space between
   the initial `'#'` and the digit characters
 - name_text does not allow whitespace or delimiters
 - string requires backslash-quoting before `'"'` and `'\'` and supports
   the usual quoted C characters (`'\\'`, `'\t'`, `'\r'`, `'\n'`)
 - tagged does not allow whitespace between syntactic subunits
 - unit does not allow whitespace between syntactic subunits
 - array, program, and list support nesting
 - `'@'` by itself starts a comment that spans to the next end of line

 Identifiers are used to represent operations and are immediately
 evaluated.

 - NOTE: This is termed a "copy" because the tokenizer passes ownership
         of any token it returns to the caller.
 */
RPL_EXPORT
rpl_token_t RPL_NULLABLE
rpl_tokenizer_copy_next(rpl_tokenizer_t tokenizer);


RPL_HEADER_END



#endif /* __RPL__rpl_tokenizer__h__ */
