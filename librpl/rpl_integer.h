//
//  rpl_integer.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_integer__h__
#define __RPL__rpl_integer__h__

#include "rpl_defines.h"

#include <stdint.h>

#include "rpl_value.h"


RPL_HEADER_BEGIN


/*! Representation of a binary integer in RPL. */
typedef uint64_t rpl_integer_t;


/*! The maximum possible binary integer value. */
#define RPL_INTEGER_MAX UINT64_MAX


/*! Create a new integer value. */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_integer_new(rpl_integer_t rep);

/*! Get the underlying representation of an integer value. */
RPL_EXPORT
rpl_integer_t
rpl_integer_get_rep(rpl_value_t val);


RPL_HEADER_END


#endif /* __RPL__rpl_integer__h__ */
