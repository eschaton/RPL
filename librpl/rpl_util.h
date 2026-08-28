//
//  rpl_util.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_defines.h"

#include "rpl_integer.h"
#include "rpl_value.h"


#ifndef __RPL__rpl_util__h__
#define __RPL__rpl_util__h__


RPL_HEADER_BEGIN


/*! Copy a C array of elements of arbitrary size. */
RPL_EXPORT
void *
rpl_util_carray_copy(void *carray, size_t carray_count,
		     size_t elem_size);


/*! Multiply all of the integers in a C array. */
RPL_EXPORT
rpl_integer_t
rpl_util_carray_integer_multily(const rpl_integer_t * const carray,
				rpl_integer_t carray_count);


RPL_HEADER_END


#endif /* __RPL__rpl_util__h__ */
