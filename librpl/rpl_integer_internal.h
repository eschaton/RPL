//
//  rpl_integer_internal.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_integer_internal__h__
#define __RPL__rpl_integer_internal__h__

#include "rpl_integer.h"


RPL_HEADER_BEGIN


RPL_EXPORT
void
rpl_integer_free(rpl_value_t integer);

/*! Round a value up to the next quantum. */
RPL_EXPORT
rpl_integer_t
rpl_integer_round_to_next(rpl_integer_t value, rpl_integer_t quantum);

RPL_EXPORT
const char * RPL_NULLABLE
rpl_integer_copy_string(rpl_value_t integer,
			rpl_environment_t RPL_NULLABLE env);

RPL_EXPORT
const char * RPL_NULLABLE
rpl_integer_rep_copy_string(rpl_integer_t integer_rep,
			    rpl_environment_t RPL_NULLABLE env);


RPL_HEADER_END


#endif /* __RPL__rpl_integer_internal__h__ */
