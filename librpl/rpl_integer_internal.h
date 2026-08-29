//
//  rpl_integer_internal.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_integer.h"


#ifndef __RPL__rpl_integer_internal__h__
#define __RPL__rpl_integer_internal__h__


RPL_HEADER_BEGIN


RPL_EXPORT
void
rpl_integer_free(rpl_value_t integer);

/*! Round a value up to the next quantum. */
RPL_EXPORT
rpl_integer_t
rpl_integer_round_to_next(rpl_integer_t value, rpl_integer_t quantum);


RPL_HEADER_END


#endif /* __RPL__rpl_integer_internal__h__ */
