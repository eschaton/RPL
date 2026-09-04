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


RPL_HEADER_BEGIN


/*! A unit of RPL syntax. */
typedef struct rpl_token *rpl_token_t;


/*! The RPL tokenizer itself. */
typedef struct rpl_tokenizer *rpl_tokenizer_t;


RPL_EXPORT
rpl_tokenizer_t RPL_NULLABLE
rpl_tokenizer_new(void);

RPL_EXPORT
void
rpl_tokenizer_free(rpl_tokenizer_t tokenizer);


RPL_HEADER_END



#endif /* __RPL__rpl_tokenizer__h__ */
