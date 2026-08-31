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

rpl_value_t RPL_NULLABLE
rpl_array_new(rpl_type_t type, rpl_integer_t capacity)
{
    assert(rpl_array_is_supported_type(type));
    assert(capacity > 0);
    
    rpl_value_t array = rpl_value_new(rpl_type_array);
    if (array) {
	rpl_array_t *rep = &array->_reps._array;
	bool initialized
	    = rpl_adjbuffer_init(&rep->_buffer, capacity,
				 rpl_array_type_size(type));
	if (!initialized) goto error;
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

bool
rpl_subarray_free_f(rpl_adjbuffer_t *buffer, void *element,
		    void * RPL_NULLABLE refcon)
{
    assert(element != NULL);

    rpl_value_t array = element;

    rpl_value_free(array);

    return true;
}

void
rpl_array_free(rpl_value_t array)
{
    assert(array != NULL);
    assert(array->_type == rpl_type_array);
    
    if (array->_reps._array._type == rpl_type_array) {
	(void) rpl_adjbuffer_apply(&array->_reps._array._buffer,
				   &rpl_subarray_free_f,
				   NULL);
    }

    rpl_adjbuffer_deinit(&array->_reps._array._buffer);
}

rpl_value_t RPL_NULLABLE
rpl_array_copy(rpl_value_t array)
{
    // TODO: rpl_array_copy

    return NULL;
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

    return rpl_adjbuffer_get_count(&array->_reps._array._buffer);
}

rpl_value_t RPL_NULLABLE
rpl_array_copy_value(rpl_value_t array, rpl_integer_t idx)
{
    rpl_value_t result;

    assert(array != NULL);
    assert(array->_type == rpl_type_array);

    rpl_adjbuffer_t *buffer = &array->_reps._array._buffer;
    void *element = rpl_adjbuffer_get(buffer, idx);

    switch (array->_reps._array._type) {
	case rpl_type_real: {
	    rpl_real_t *real = element;
	    result = rpl_real_new(*real);
	} break;

	case rpl_type_complex: {
	    rpl_complex_t *complex = element;
	    result = rpl_complex_new(complex->_a, complex->_b);
	} break;

	case rpl_type_array: {
	    rpl_value_t subarray = element;
	    result = rpl_value_retain(subarray);
	} break;

	default: {
	    result = NULL;
	} break;
    }

    return result;
}

void
rpl_array_set_value(rpl_value_t array, rpl_integer_t idx,
		    rpl_value_t value)
{
    assert(array != NULL);
    assert(array->_type == rpl_type_array);

    rpl_adjbuffer_t *buffer = &array->_reps._array._buffer;

    switch (array->_reps._array._type) {
	case rpl_type_real: {
	    assert(value->_type == rpl_type_real);
	    rpl_real_t *rep = &value->_reps._real;
	    rpl_adjbuffer_set(buffer, idx, rep);
	} break;

	case rpl_type_complex: {
	    assert(value->_type == rpl_type_complex);
	    rpl_complex_t *rep = &value->_reps._complex;
	    rpl_adjbuffer_set(buffer, idx, rep);
	} break;
	    
	case rpl_type_array: {
	    assert(value->_type == rpl_type_array);
	    /*
	     For an array, we need to:
	     - ensure the new value is retained
	     - get the existing value
	     - replace the existing value with the new value
	     - release the existing value
	     */
	    rpl_value_retain(value);
	    void *element = rpl_adjbuffer_get(buffer, idx);
	    rpl_value_t *evalue_ptr = element;
	    assert(evalue_ptr != NULL);
	    rpl_value_t evalue = *evalue_ptr;
	    rpl_adjbuffer_set(buffer, idx, &value);
	    rpl_value_release(evalue);
	} break;
	    
	default: {
	    assert(0);
	} break;
    }
}

bool
rpl_array_insert_value(rpl_value_t array, rpl_integer_t idx,
		       rpl_value_t value)
{
    assert(array != NULL);
    assert(array->_type == rpl_type_array);

    rpl_adjbuffer_t *buffer = &array->_reps._array._buffer;
    void *rep = NULL;

    switch (array->_reps._array._type) {
	case rpl_type_real: {
	    assert(value->_type == rpl_type_real);
	    rep = &value->_reps._real;
	} break;

	case rpl_type_complex: {
	    assert(value->_type == rpl_type_complex);
	    rep = &value->_reps._complex;
	} break;

	case rpl_type_array: {
	    assert(value->_type == rpl_type_array);
	    rpl_value_retain(value);
	    rep = &value;
	} break;

	default: {
	    assert(0);
	} break;
    }

    return rpl_adjbuffer_insert(buffer, idx, rep);
}

void
rpl_array_remove_value(rpl_value_t array, rpl_integer_t idx)
{
    assert(array != NULL);
    assert(array->_type == rpl_type_array);

    rpl_adjbuffer_t *buffer = &array->_reps._array._buffer;

    switch (array->_reps._array._type) {
	case rpl_type_real:
	case rpl_type_complex:
	    /* Do nothing. */
	    break;

	case rpl_type_array: {
	    void *element = rpl_adjbuffer_get(buffer, idx);
	    rpl_value_t *evalue_ptr = element;
	    assert(evalue_ptr != NULL);
	    rpl_value_t evalue = *evalue_ptr;
	    rpl_value_release(evalue);
	} break;

	default: {
	    assert(0);
	} break;
    }

    rpl_adjbuffer_remove(buffer, idx);
}

bool
rpl_array_append_value(rpl_value_t array, rpl_value_t value)
{
    assert(array != NULL);
    assert(array->_type == rpl_type_array);
    assert(value != NULL);
    assert(value->_type == array->_reps._array._type);

    rpl_adjbuffer_t *buffer = &array->_reps._array._buffer;
    void *rep = NULL;

    switch (array->_reps._array._type) {
	case rpl_type_real: {
	    rep = &value->_reps._real;
	} break;

	case rpl_type_complex: {
	    rep = &value->_reps._complex;
	} break;

	case rpl_type_array: {
	    rep = &value;
	} break;

	default: {
	    assert(0);
	} break;
    }

    return rpl_adjbuffer_append(buffer, rep);
}


RPL_SOURCE_END
