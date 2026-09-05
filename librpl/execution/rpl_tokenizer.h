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

/*! Get the next token, if any, from the tokenizer. */
RPL_EXPORT
rpl_token_t RPL_NULLABLE
rpl_tokenizer_get_next(rpl_tokenizer_t tokenizer);


RPL_HEADER_END



#endif /* __RPL__rpl_tokenizer__h__ */
