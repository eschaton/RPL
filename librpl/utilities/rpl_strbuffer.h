//
//  rpl_strbuffer.h
//  RPL
//
//  Created by Chris Hanson on 9/2/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_strbuffer__h__
#define __RPL__rpl_strbuffer__h__

#include "rpl_defines.h"

#include <stdbool.h>
#include <unistd.h>


RPL_HEADER_BEGIN


/*! A buffer specialized for storing strings. */
typedef struct rpl_strbuffer *rpl_strbuffer_t;


rpl_strbuffer_t RPL_NULLABLE
rpl_strbuffer_new_empty(size_t capacity);

rpl_strbuffer_t RPL_NULLABLE
rpl_strbuffer_new(const char *str);

void
rpl_strbuffer_free(rpl_strbuffer_t sb);

const char *
rpl_strbuffer_get_chars(rpl_strbuffer_t sb);

const char * RPL_NULLABLE
rpl_strbuffer_copy_chars(rpl_strbuffer_t sb);

size_t
rpl_strbuffer_get_length(rpl_strbuffer_t sb);

bool
rpl_strbuffer_append_chars(rpl_strbuffer_t sb, const char *str);

bool
rpl_strbuffer_append_strbuffer(rpl_strbuffer_t sb,
			       const rpl_strbuffer_t sb_appended);


RPL_HEADER_END


#endif /* __RPL__rpl_strbuffer__h__ */
