//
//  rpl_tokenizer.c
//  RPL
//
//  Created by Chris Hanson on 9/3/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_tokenizer_internal.h"

#include <assert.h>
#include <stdlib.h>


RPL_SOURCE_BEGIN


rpl_tokenizer_t RPL_NULLABLE
rpl_tokenizer_new(void)
{
    rpl_tokenizer_t tokenizer = calloc(1, sizeof(struct rpl_tokenizer));
    if (tokenizer) {

    }

    return tokenizer;
}

RPL_EXPORT
void
rpl_tokenizer_free(rpl_tokenizer_t tokenizer)
{
    assert(tokenizer != NULL);

    free(tokenizer);
}


RPL_SOURCE_END
