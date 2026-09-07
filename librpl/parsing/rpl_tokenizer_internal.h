//
//  rpl_tokenizer_internal.h
//  RPL
//
//  Created by Chris Hanson on 9/3/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_tokenizer_internal__h__
#define __RPL__rpl_tokenizer_internal__h__

#include "rpl_tokenizer.h"

#include "rpl_strbuffer.h"


RPL_HEADER_BEGIN


struct rpl_tokenizer {
    rpl_context_t _context;
    rpl_strbuffer_t _strbuffer;
    ssize_t _cur;
};

char
rpl_tokenizer_get_char(rpl_tokenizer_t tokenizer);

void
rpl_tokenizer_unget_char(rpl_tokenizer_t tokenizer, char ch);

char
rpl_tokenizer_peek_char(rpl_tokenizer_t tokenizer);

bool
rpl_tokenizer_has_char(rpl_tokenizer_t tokenizer);

ssize_t
rpl_tokenizer_get_mark(rpl_tokenizer_t tokenizer);

void
rpl_tokenizer_set_mark(rpl_tokenizer_t tokenizer, ssize_t mark);


RPL_HEADER_END


#endif /* __RPL__rpl_tokenizer_internal__h__ */
