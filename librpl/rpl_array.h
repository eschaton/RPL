//
//  rpl_array.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_defines.h"

#include "rpl_integer.h"
#include "rpl_value.h"


#ifndef __RPL__rpl_array__h__
#define __RPL__rpl_array__h__


RPL_HEADER_BEGIN


/*!
 Create a new n-dimensional array.

 - NOTE: The `vals_count` **must** be equal to the product of the values
	 of `dims`, and `vals` must in memory-indexing order.
 */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_array_new(rpl_integer_t *dims, rpl_integer_t dims_count,
	      rpl_value_t RPL_NONNULL * RPL_NONNULL vals,
	      rpl_integer_t vals_count);

/*! Get the number of dimensions of an n-dimensional array. */
RPL_EXPORT
rpl_integer_t
rpl_array_get_dimensions_count(rpl_value_t array);

/*! Get the dimensions of an n-dimensional array.

 - WARNING: The array of dimensions remains owned by the array.
 */
RPL_EXPORT
rpl_integer_t *
rpl_array_get_dimensions(rpl_value_t array);

/*!
 Get a value within an n-dimensional array.

 - WARNING: The number of indidecs **must** match the number of
	    dimensions in the array.
 */
RPL_EXPORT
rpl_value_t
rpl_array_get(rpl_value_t array, rpl_integer_t *indices);

/*!
 Set a value within an n-dimensional array.

 - WARNING: The number of indidecs **must** match the number of
	    dimensions in the array.
 */
RPL_EXPORT
void
rpl_array_set(rpl_value_t array, rpl_integer_t *indices,
	      rpl_value_t value);


RPL_HEADER_END


#endif /* __RPL__rpl_array__h__ */
