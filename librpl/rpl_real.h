//
//  rpl_real.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_real__h__
#define __RPL__rpl_real__h__


#include "rpl_defines.h"

#include "rpl_value.h"

RPL_HEADER_BEGIN


/*! Representation of a real number in RPL. */
typedef double rpl_real_t;

/*! Create a new real value. */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_real_new(rpl_real_t rep);

/*! Get the underlying representation of an integer value. */
RPL_EXPORT
rpl_real_t
rpl_real_get_rep(rpl_value_t val);


RPL_HEADER_END


#endif /* __RPL__rpl_real__h__ */
