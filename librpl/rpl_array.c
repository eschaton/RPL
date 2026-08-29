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
#include <string.h>

#include "rpl_util.h"
#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN


const rpl_integer_t rpl_array_quantum = 16;


bool
rpl_array_is_supported_type(rpl_type_t type)
{
    switch (type) {
	case rpl_type_real:
	case rpl_type_complex:
	case rpl_type_array:
	    return true;

	case rpl_type_integer:
	case rpl_type_name:
	case rpl_type_program:
	case rpl_type_string:
	case rpl_type_list:
	case rpl_type_tagged:
	case rpl_type_unit:
	    return false;
    }
}

size_t
rpl_array_type_size(rpl_type_t type)
{
    switch (type) {
	case rpl_type_real:    return sizeof(rpl_real_t);
	case rpl_type_complex: return sizeof(rpl_complex_t);
	case rpl_type_array:   return sizeof(rpl_value_t);
	    
	case rpl_type_integer:
	case rpl_type_name:
	case rpl_type_program:
	case rpl_type_string:
	case rpl_type_list:
	case rpl_type_tagged:
	case rpl_type_unit:
	    return 0;
    }
}

/*!
 Adjust the storage of an array if adding one more entry would fill it.
 
 @returns `false` if allocation failed, `true` otherwise
 */
bool
rpl_array_adjust_storage(rpl_value_t array)
{
    assert(array != NULL);
    assert(array->_type == rpl_type_array);
    
    rpl_array_t *rep = &array->_reps._array;
    if (rep->_count == (rep->_capacity - 1)) {
	rpl_integer_t ocap = rep->_capacity;
	rpl_integer_t ncap = ocap + rpl_array_quantum;
	size_t osize = ocap * rpl_array_type_size(rep->_type);
	size_t nsize = ncap * rpl_array_type_size(rep->_type);
	void *ovalues = rep->_values;
	void *nvalues = realloc(ovalues, nsize);
	if (nvalues == NULL) goto error;
	uint8_t *obytes = ovalues;
	memset(&obytes[osize], 0, nsize - osize);
    }
    return true;
    
error:
    return false;
}

rpl_value_t RPL_NULLABLE
rpl_array_new(rpl_type_t type, rpl_integer_t capacity)
{
    assert(rpl_array_is_supported_type(type));
    assert(capacity > 0);
    
    rpl_value_t array = rpl_value_new(rpl_type_array);
    if (array) {
	const rpl_integer_t real_capacity
	    = rpl_integer_round_to_next(capacity, rpl_array_quantum);
	
	const size_t value_size = rpl_array_type_size(type);
	assert(value_size > 0);
	
	rpl_array_t *rep = &array->_reps._array;
	rep->_values = calloc(real_capacity, value_size);
	if (rep->_values == NULL) goto error;
	rep->_count = 0;
	rep->_capacity = capacity;
	rep->_type = type;
    }
    return array;
    
error:
    rpl_value_release(array);
    return NULL;
}

rpl_value_t RPL_NULLABLE
rpl_array_new_with_values(rpl_type_t type,
			  rpl_value_t RPL_NONNULL * RPL_NONNULL vals,
			  rpl_integer_t vals_count)
{
    rpl_value_t array = rpl_array_new(type, vals_count);
    if (array) {
	// TODO: rpl_array_new_with_values
    }
    return array;
}

void
rpl_array_free(rpl_value_t array)
{
    assert(array != NULL);
    assert(array->_type == rpl_type_array);
    
    if (array->_reps._array._type == rpl_type_array) {
	rpl_value_t *values = array->_reps._array._values;
	rpl_integer_t count = array->_reps._array._count;
	rpl_value_release_array(values, count);
    }
    
    free(array->_reps._array._values);
}

rpl_type_t
rpl_array_get_type(rpl_value_t array)
{
    assert(array != NULL);
    assert(array->_type == rpl_type_array);

    return array->_reps._array._type;
}

rpl_integer_t
rpl_array_get_count(rpl_value_t array)
{
    assert(array != NULL);
    assert(array->_type == rpl_type_array);

    return array->_reps._array._count;
}

rpl_value_t RPL_NULLABLE
rpl_array_copy_value(rpl_value_t array, rpl_integer_t index)
{
    rpl_value_t result;

    assert(array != NULL);
    assert(array->_type == rpl_type_array);
    assert(index < array->_reps._array._count);

    switch (array->_reps._array._type) {
	case rpl_type_real: {
	    rpl_real_t *elements = array->_reps._array._values;
	    rpl_real_t element = elements[index];
	    result = rpl_real_new(element);
	} break;
	    
	case rpl_type_complex: {
	    rpl_complex_t *elements = array->_reps._array._values;
	    rpl_complex_t element = elements[index];
	    result = rpl_complex_new(element._a, element._b);
	} break;
	    
	case rpl_type_array: {
	    rpl_value_t *elements = array->_reps._array._values;
	    rpl_value_t element = elements[index];
	    result = element;
	} break;
	    
	default: {
	    result = NULL;
	} break;
    }

    return result;
}

void
rpl_array_set_value(rpl_value_t array, rpl_integer_t index,
		    rpl_value_t value)
{
    assert(array != NULL);
    assert(array->_type == rpl_type_array);
    assert(index < array->_reps._array._count);
    
    switch (array->_reps._array._type) {
	case rpl_type_real: {
	    assert(value->_type == rpl_type_real);
	    rpl_real_t *elements = array->_reps._array._values;
	    elements[index] = value->_reps._real;
	} break;
	    
	case rpl_type_complex: {
	    assert(value->_type == rpl_type_complex);
	    rpl_complex_t *elements = array->_reps._array._values;
	    elements[index] = array->_reps._complex;
	} break;
	    
	case rpl_type_array: {
	    assert(value->_type == rpl_type_array);
	    rpl_value_t *elements = array->_reps._array._values;
	    rpl_value_t element = elements[index];
	    rpl_value_retain(value);
	    elements[index] = value;
	    rpl_value_release(element);
	} break;
	    
	default: {
	    assert(0);
	} break;
    }
}

bool
rpl_array_insert_value(rpl_value_t array, rpl_integer_t index,
		       rpl_value_t value)
{
    assert(array != NULL);
    assert(array->_type == rpl_type_array);
    assert(value != NULL);
    assert(value->_type == array->_reps._array._type);
    assert(index <= array->_reps._array._count);

    bool adjusted = rpl_array_adjust_storage(array);
    if (adjusted == false) goto error;
    
    // TODO: rpl_array_insert_value
    
    return false;
    
error:
    return false;
}

void
rpl_array_remove_value(rpl_value_t array, rpl_integer_t index)
{
    assert(array != NULL);
    assert(array->_type == rpl_type_array);
    assert(index < array->_reps._array._count);

    // TODO: rpl_array_remove_value
}

bool
rpl_array_append_value(rpl_value_t array, rpl_value_t value)
{
    assert(array != NULL);
    assert(array->_type == rpl_type_array);
    assert(value != NULL);
    assert(value->_type == array->_reps._array._type);

    return rpl_array_insert_value(array, array->_reps._array._count,
				  value);
}


RPL_SOURCE_END
