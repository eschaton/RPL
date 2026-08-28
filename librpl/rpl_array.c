//
//  rpl_array.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_array_internal.h"

#include <assert.h>
#include <stdlib.h>

#include "rpl_util.h"
#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN


/*
 Arrays in RPL are multidimensional and mutable in their contents but
 not dimensionality, so they are represented by an underlying C array
 with a single dimension.
 */

rpl_value_t RPL_NULLABLE
rpl_array_new(rpl_integer_t *dims, rpl_integer_t dims_count,
	      rpl_value_t RPL_NONNULL * RPL_NONNULL vals,
	      rpl_integer_t vals_count)
{
    assert(rpl_util_carray_integer_multily(dims, dims_count)
	   == vals_count);

    rpl_value_t array = rpl_value_new(rpl_type_array);
    if (array) {
	array->_reps._array._dims_count = dims_count;
	array->_reps._array._dims
	    = rpl_util_carray_copy(dims, dims_count,
				   sizeof(rpl_integer_t));
	if (array->_reps._array._dims == NULL) goto error;

	array->_reps._array._vals_count = vals_count;
	array->_reps._array._vals
	    = rpl_util_carray_copy(vals, vals_count,
				   sizeof(rpl_value_t));
	if (array->_reps._array._vals == NULL) goto error;
    }
    return array;

error:
    rpl_value_free(array);
    return NULL;
}

void
rpl_array_free(rpl_value_t array)
{
    assert(array != NULL);
    assert(array->_type == rpl_type_array);

    free(array->_reps._array._dims);

    if (array->_reps._array._vals) {
	rpl_value_free_array(array->_reps._array._vals,
			     array->_reps._array._vals_count);
    }
}

rpl_integer_t
rpl_array_get_dimensions_count(rpl_value_t array)
{
    assert(array != NULL);
    assert(array->_type == rpl_type_array);

    return array->_reps._array._dims_count;
}

rpl_integer_t *
rpl_array_get_dimensions(rpl_value_t array)
{
    assert(array != NULL);
    assert(array->_type == rpl_type_array);

    return array->_reps._array._dims;
}

/*!
 Compute the offset of an element a one-dimensional array being used
 to represent a multidimensional array.

 Take a 2-dimensional array with `[x,y]` dimensions `[3,4]` arranged
 in the underlying 1-dimensional representation with `3*4=12` memory
 locations like so:

      0  1  2
      3  4  5
      6  7  8
      9 10 11

 Then the zero-based index `(1,2)` corresponds to the memory location at
 `(2*3)+1=7`.

 Similarly, take a 3-dimensional array with dimensions of `[2,3,4]`
 which is arranged in `2*3*4=24` conecutive memory locations:

      0  1    6  7   12 13   18 19
      2  3    8  9   14 15   20 21
      4  5   10 11   16 17   22 23

 Then the zero-based index `(1,2,3)` corresponds to location
 `((3*3*2)+(2*2)+1*=23`, i.e. `(i2*d1*d0)+(i1*d0)+i0`.

 This should make clear the generalized algorithm.
 */
rpl_integer_t
rpl_array_offset_for_indices(rpl_value_t array, rpl_integer_t *indices)
{
    assert(array != NULL);
    assert(array->_type == rpl_type_array);

    const rpl_integer_t * const dims = array->_reps._array._dims;
    const rpl_integer_t dims_count = array->_reps._array._dims_count;

    rpl_integer_t offset = 0;
    for (rpl_integer_t i = dims_count - 1; i > 0; i--) {
	rpl_integer_t current = indices[i];
	current *= rpl_util_carray_integer_multily(dims, i);
	offset += current;
    }
    offset += indices[0]; // i=0 case

    if (offset >= array->_reps._array._vals_count) {
	offset = -1;
    }

    return offset;
}

rpl_value_t
rpl_array_get(rpl_value_t array, rpl_integer_t *indices)
{
    rpl_value_t result = NULL;

    assert(array != NULL);
    assert(array->_type == rpl_type_array);
    assert(indices != NULL);

    rpl_integer_t offset = rpl_array_offset_for_indices(array, indices);
    assert(offset >= 0);

    result = array->_reps._array._vals[offset];

    return result;
}

void
rpl_array_set(rpl_value_t array, rpl_integer_t *indices,
	      rpl_value_t value)
{
    assert(array != NULL);
    assert(array->_type == rpl_type_array);
    assert(indices != NULL);

    rpl_integer_t offset = rpl_array_offset_for_indices(array, indices);
    assert(offset >= 0);

    array->_reps._array._vals[offset] = value;
}


RPL_SOURCE_END
