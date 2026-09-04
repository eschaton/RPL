//
//  rpl_array.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_array__h__
#define __RPL__rpl_array__h__

#include "rpl_defines.h"

#include <stdbool.h>

#include "rpl_integer.h"
#include "rpl_value.h"


RPL_HEADER_BEGIN


/*! Indicate whether a type is supported in arrays. */
RPL_EXPORT
bool
rpl_array_is_supported_type(rpl_type_t type);

/*! Create a new RPL array. */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_array_new(rpl_type_t type, rpl_integer_t capacity);

/*!
 Create a new RPL array with an existing set of values.
 
 - NOTE: If the values are arrays, this array will take ownership of
         them rather than copy them.
 */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_array_new_with_values(rpl_type_t type,
			  rpl_value_t RPL_NONNULL * RPL_NONNULL vals,
			  rpl_integer_t vals_count);

/*! Get the type of the elements in the array. */
RPL_EXPORT
rpl_type_t
rpl_array_get_type(rpl_value_t array);

/*! Get the number of elements in the array. */
RPL_EXPORT
rpl_integer_t
rpl_array_get_count(rpl_value_t array);

/*! Get a value from an array.
 
 - WARNING: For all values, the caller must release; this is why the
            function uses `copy` rather than `get` in its name.
 */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_array_copy_value(rpl_value_t array, rpl_integer_t idx);

/*! Set a value within an array. */
RPL_EXPORT
void
rpl_array_set_value(rpl_value_t array, rpl_integer_t idx,
		    rpl_value_t value);

/*!
 Insert a value into an array.
 
 @returns `false` on allocation failure
 */
RPL_EXPORT
bool
rpl_array_insert_value(rpl_value_t array, rpl_integer_t idx,
		       rpl_value_t value);

/*! Remove a value from an array. */
RPL_EXPORT
void
rpl_array_remove_value(rpl_value_t array, rpl_integer_t idx);

/*! Append a value to an array. */
RPL_EXPORT
bool
rpl_array_append_value(rpl_value_t array, rpl_value_t value);


RPL_HEADER_END


#endif /* __RPL__rpl_array__h__ */
