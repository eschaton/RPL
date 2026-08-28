//
//  rpl_value.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_value_internal.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


RPL_SOURCE_BEGIN


/* MARK: - Utilty */

/*! Copy a C array of elements of arbitrary size. */
void *
rpl_util_carray_copy(void *carray, size_t carray_count,
		     size_t elem_size)
{
    assert(carray != NULL);
    assert(carray_count > 0);
    assert(elem_size > 0);

    const size_t size = carray_count * elem_size;
    void *result = malloc(size);
    if (result) {
	memcpy(result, carray, size);
    }

    return result;
}

rpl_integer_t
rpl_util_carray_integer_multily(const rpl_integer_t * const carray,
				rpl_integer_t carray_count)
{
    assert(carray != NULL);
    assert(carray_count > 0);

    rpl_integer_t result = 1;
    for (size_t i = 0; i < carray_count; i++) {
	result *= carray[i];
    }

    return result;
}


/* MARK: - Value */

rpl_value_t RPL_NULLABLE
rpl_value_new(rpl_type_t type)
{
    rpl_value_t val = calloc(1, sizeof(struct rpl_value));
    if (val) {
	val->_type = type;
    }
    return val;
}

void
rpl_value_free(rpl_value_t val)
{
    assert(val != NULL);

    switch (val->_type) {
	case rpl_type_integer: rpl_integer_free(val); break;
	case rpl_type_real:    rpl_real_free(val);    break;
	case rpl_type_complex: rpl_complex_free(val); break;
	case rpl_type_array:   rpl_array_free(val);   break;
	case rpl_type_name:    rpl_name_free(val);    break;
	case rpl_type_program: rpl_program_free(val); break;
	case rpl_type_string:  rpl_string_free(val);  break;
	case rpl_type_list:    rpl_list_free(val);    break;
	case rpl_type_tagged:  rpl_tagged_free(val);  break;
	case rpl_type_unit:    rpl_unit_free(val);    break;
    }

    free(val);
}

void
rpl_value_free_array(rpl_value_t *vals, rpl_integer_t vals_count)
{
    assert(vals != NULL);
    assert(vals_count > 0);

    for (rpl_integer_t i = 0; i < vals_count; i++) {
	rpl_value_t val = vals[i];
	rpl_value_free(val);
    }
}

rpl_type_t
rpl_value_get_type(rpl_value_t val)
{
    return val->_type;
}


/* MARK: - Integer */

rpl_value_t RPL_NULLABLE
rpl_integer_new(rpl_integer_t rep)
{
    rpl_value_t val = rpl_value_new(rpl_type_integer);
    if (val) {
	val->_reps._integer = rep;
    }
    return val;
}

void
rpl_integer_free(rpl_value_t integer)
{
    assert(integer != NULL);
    assert(integer->_type == rpl_type_integer);

    /* Nothing to do. */
}

rpl_integer_t
rpl_integer_get_rep(rpl_value_t integer)
{
    assert(integer != NULL);
    assert(integer->_type == rpl_type_integer);

    return integer->_reps._integer;
}


/* MARK: - Real */

rpl_value_t RPL_NULLABLE
rpl_real_new(rpl_real_t rep)
{
    rpl_value_t val = rpl_value_new(rpl_type_real);
    if (val) {
	val->_reps._real = rep;
    }
    return val;
}

void
rpl_real_free(rpl_value_t real)
{
    assert(real != NULL);
    assert(real->_type == rpl_type_real);

    /* Nothing to do. */
}

rpl_integer_t
rpl_real_get_rep(rpl_value_t real)
{
    assert(real != NULL);
    assert(real->_type == rpl_type_real);

    return real->_reps._real;
}


/* MARK: - Complex */

rpl_value_t RPL_NULLABLE
rpl_complex_rectangular_new(rpl_real_t x_rep, rpl_real_t y_rep)
{
    rpl_value_t complex = rpl_value_new(rpl_type_complex);
    if (complex) {
	complex->_reps._complex._is_polar = false;
	complex->_reps._complex._value._rectangular._x = x_rep;
	complex->_reps._complex._value._rectangular._y = y_rep;
    }
    return complex;
}

rpl_value_t RPL_NULLABLE
rpl_complex_polar_new(rpl_real_t r_rep, rpl_real_t theta_rep)
{
    rpl_value_t complex = rpl_value_new(rpl_type_complex);
    if (complex) {
	complex->_reps._complex._is_polar = true;
	complex->_reps._complex._value._polar._r = r_rep;
	complex->_reps._complex._value._polar._theta = theta_rep;
    }
    return complex;
}

void
rpl_complex_free(rpl_value_t complex)
{
    assert(complex != NULL);
    assert(complex->_type == rpl_type_complex);

    /* Nothing to do. */
}

// TODO: Complex Number Operations


/* MARK: - Array */

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


/* MARK: - Name */

/*
 TODO: Name interning mechanism

 Names should be interned and reference counted, so using the same names
 in a lot of different situations simultaneously is efficient.
 */

rpl_value_t RPL_NULLABLE
rpl_name_new(const char *rep, rpl_integer_t rep_len)
{
    assert(strlen(rep) == rep_len);

    rpl_value_t name = rpl_value_new(rpl_type_name);
    if (name) {
	name->_reps._name._chars = strdup(rep);
	if (name->_reps._name._chars == NULL) goto error;

	name->_reps._name._chars_len = rep_len;
    }
    return name;

error:
    rpl_value_free(name);
    return NULL;
}

void
rpl_name_free(rpl_value_t name)
{
    assert(name != NULL);
    assert(name->_type == rpl_type_name);

    free(name->_reps._name._chars);
}

const char *
rpl_name_get_rep(rpl_value_t name)
{
    assert(name != NULL);
    assert(name->_type == rpl_type_name);

    return name->_reps._name._chars;
}

rpl_integer_t
rpl_name_get_rep_len(rpl_value_t name)
{
    assert(name != NULL);
    assert(name->_type == rpl_type_name);

    return name->_reps._name._chars_len;
}


/* MARK: - Program */

void
rpl_program_free(rpl_value_t program)
{
    assert(program != NULL);
    assert(program->_type == rpl_type_program);

    // TODO: Free program
}

// TODO: Program Operations


/* MARK: - String */

rpl_value_t RPL_NULLABLE
rpl_string_new(const char *rep, rpl_integer_t rep_len)
{
    assert(rep != NULL);
    assert(strlen(rep) == rep_len);

    rpl_value_t string = rpl_value_new(rpl_type_string);
    if (string) {
	string->_reps._string._chars = strdup(rep);
	if (string->_reps._string._chars == NULL) goto error;

	string->_reps._string._chars_len = rep_len;
    }
    return string;

error:
    rpl_value_free(string);
    return NULL;
}

void
rpl_string_free(rpl_value_t string)
{
    assert(string != NULL);
    assert(string->_type == rpl_type_string);

    free(string->_reps._string._chars);
}

const char *
rpl_string_get_rep(rpl_value_t string)
{
    assert(string != NULL);
    assert(string->_type == rpl_type_string);

    return string->_reps._string._chars;
}

rpl_integer_t
rpl_string_get_rep_len(rpl_value_t string)
{
    assert(string != NULL);
    assert(string->_type == rpl_type_string);

    return string->_reps._string._chars_len;
}


/* MARK: - List */

rpl_value_t RPL_NULLABLE
rpl_list_new(void)
{
    return rpl_value_new(rpl_type_list);
}

rpl_value_t RPL_NULLABLE
rpl_list_new_with_pair(rpl_value_t RPL_NULLABLE car,
		       rpl_value_t RPL_NULLABLE cdr)
{
    rpl_value_t list = rpl_list_new();
    if (list) {
	list->_reps._list._car = car;
	list->_reps._list._cdr = cdr;
    }
    return list;
}

rpl_value_t RPL_NULLABLE
rpl_list_new_with_array(rpl_value_t RPL_NONNULL * RPL_NONNULL elements,
			rpl_integer_t elements_count)
{
    assert(elements != NULL);
    assert(elements_count > 0);

    rpl_value_t list = rpl_list_new();
    if (list) {
	list->_reps._list._car = elements[0];
	rpl_value_t current = list;
	for (rpl_integer_t i = 1; i < elements_count; i++) {
	    rpl_value_t element = elements[i];
	    assert(element != NULL);

	    rpl_value_t pair = rpl_list_new_with_pair(element, NULL);
	    if (pair == NULL) goto error;

	    current->_reps._list._cdr = pair;
	    current = pair;
	}
    }
    return list;

error:
    rpl_value_free(list);
    return list;
}

void
rpl_list_free(rpl_value_t list)
{
    assert(list != NULL);
    assert(list->_type == rpl_type_list);

    // TODO: Free list
}

rpl_value_t
rpl_list_car(rpl_value_t list)
{
    assert(list != NULL);
    assert(list->_type == rpl_type_list);

    return list->_reps._list._car;
}

rpl_value_t
rpl_list_cdr(rpl_value_t val)
{
    assert(val != NULL);
    assert(val->_type == rpl_type_list);

    return val->_reps._list._cdr;
}

void
rpl_list_append(rpl_value_t list, rpl_value_t list2)
{
    assert(list != NULL);
    assert(list->_type == rpl_type_list);
    assert(list2 != NULL);
    assert(list2->_type == rpl_type_list);

    /* Follow list's cdr to a node that has a NULL cdr. */

    rpl_value_t next = list;
    rpl_value_t tail = NULL;
    do {
	assert(next->_type == rpl_type_list);
	if (next->_reps._list._cdr == NULL) {
	    tail = next;
	}
	next = next->_reps._list._cdr;
    } while (tail != NULL);

    /* Set the tail's cdr to val. */

    tail->_reps._list._cdr = list2;
}


/* MARK: - Tagged*/

rpl_value_t RPL_NULLABLE
rpl_tagged_new(rpl_value_t tag, rpl_value_t value)
{
    assert(tag != NULL);
    assert(tag->_type == rpl_type_name);
    assert(value != NULL);

    rpl_value_t tagged = rpl_value_new(rpl_type_tagged);
    if (tagged) {
	tagged->_reps._tagged._tag = tag;
	tagged->_reps._tagged._value = value;
    }
    return tagged;
}

void
rpl_tagged_free(rpl_value_t tagged)
{
    assert(tagged != NULL);
    assert(tagged->_type == rpl_type_tagged);

    rpl_value_free(tagged->_reps._tagged._tag);
    rpl_value_free(tagged->_reps._tagged._value);
}

rpl_value_t
rpl_tagged_get_tag(rpl_value_t tagged)
{
    assert(tagged != NULL);
    assert(tagged->_type == rpl_type_tagged);

    return tagged->_reps._tagged._tag;
}

rpl_value_t
rpl_tagged_get_value(rpl_value_t tagged)
{
    assert(tagged != NULL);
    assert(tagged->_type == rpl_type_tagged);

    return tagged->_reps._tagged._value;
}


/* MARK: - Unit */

rpl_value_t RPL_NULLABLE
rpl_unit_new(rpl_real_t scalar, rpl_value_t units)
{
    assert(units != NULL);
    assert(units->_type == rpl_type_name);

    rpl_value_t unit = rpl_value_new(rpl_type_unit);
    if (unit) {
	unit->_reps._unit._scalar = scalar;
	unit->_reps._unit._units = units;
    }
    return unit;
}

void
rpl_unit_free(rpl_value_t unit)
{
    assert(unit != NULL);
    assert(unit->_type == rpl_type_unit);

    rpl_value_free(unit->_reps._unit._units);
}

rpl_real_t
rpl_unit_get_scalar_rep(rpl_value_t unit)
{
    assert(unit != NULL);
    assert(unit->_type == rpl_type_program);

    return unit->_reps._unit._scalar;
}

rpl_value_t
rpl_unit_get_units(rpl_value_t unit)
{
    assert(unit != NULL);
    assert(unit->_type == rpl_type_program);

    return unit->_reps._unit._units;
}


RPL_SOURCE_END
