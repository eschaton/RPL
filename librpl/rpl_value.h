//
//  rpl_value.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_value__h__
#define __RPL__rpl_value__h__

#include <stdbool.h>
#include <stdint.h>

#include "rpl_defines.h"


RPL_HEADER_BEGIN


/* MARK: RPL Types */

/*! The types of values RPL supports. */
enum rpl_type {
    rpl_type_integer,
    rpl_type_real,
    rpl_type_complex,
    rpl_type_array,
    rpl_type_name,
    rpl_type_program,
    rpl_type_string,
    rpl_type_list,
    rpl_type_tagged,
    rpl_type_unit,
};
typedef enum rpl_type rpl_type_t;


/* MARK: RPL Values */

/*! A typed value in RPL. */
typedef struct rpl_value *rpl_value_t;


/*! Free an allocated value. */
RPL_EXPORT
void
rpl_value_free(rpl_value_t val);

/*! Get the type of a value. */
rpl_type_t
rpl_value_get_type(rpl_value_t val);


/* MARK: - Integer */

/*! Representation of an integer in RPL. */
typedef int64_t rpl_integer_t;

/*! Create a new integer value. */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_integer_new(rpl_integer_t rep);

/*! Get the underlying representation of an integer value. */
RPL_EXPORT
rpl_integer_t
rpl_integer_get_rep(rpl_value_t val);


/* MARK: - Real */

/*! Representation of a real number in RPL. */
typedef double rpl_real_t;

/*! Create a new real value. */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_real_new(rpl_real_t rep);

/*! Get the underlying representation of an integer value. */
RPL_EXPORT
rpl_integer_t
rpl_integer_get_rep(rpl_value_t val);


/* MARK: - Complex */

/*! Create a new rectangular complex value. */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_complex_rectangular_new(rpl_real_t x_rep, rpl_real_t y_rep);

/*! Create a new polar complex value. */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_complex_polar_new(rpl_real_t r_rep, rpl_real_t theta_rep);

// TODO: Complex Number Operations


/* MARK: - Array */

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


/* MARK: - Name */

RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_name_new(const char *rep, rpl_integer_t rep_len);

RPL_EXPORT
const char *
rpl_name_get_rep(rpl_value_t val);

RPL_EXPORT
rpl_integer_t
rpl_name_get_rep_len(rpl_value_t val);


/* MARK: - Program */

// TODO: Program operations.


/* MARK: - String */

RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_string_new(const char *rep, rpl_integer_t rep_len);

RPL_EXPORT
const char *
rpl_string_get_rep(rpl_value_t val);

RPL_EXPORT
rpl_integer_t
rpl_string_get_rep_len(rpl_value_t val);

// TODO: Other string operations.


/* MARK: - List */

RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_list_new(void);

RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_list_new_with_pair(rpl_value_t RPL_NULLABLE car,
		       rpl_value_t RPL_NULLABLE cdr);

RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_list_new_with_array(rpl_value_t RPL_NONNULL * RPL_NONNULL elements,
			rpl_integer_t elements_count);

RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_list_car(rpl_value_t val);

RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_list_cdr(rpl_value_t val);

RPL_EXPORT
void
rpl_list_append(rpl_value_t list, rpl_value_t list2);


/* MARK: - Tagged*/

RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_tagged_new(rpl_value_t tag, rpl_value_t value);

RPL_EXPORT
rpl_value_t
rpl_tagged_get_tag(rpl_value_t tagged);

RPL_EXPORT
rpl_value_t
rpl_tagged_get_value(rpl_value_t tagged);


/* MARK: - Unit */

RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_unit_new(rpl_real_t scalar, rpl_value_t units);

RPL_EXPORT
rpl_real_t
rpl_unit_get_scalar_rep(rpl_value_t units);

RPL_EXPORT
rpl_value_t
rpl_unit_get_units(rpl_value_t units);


RPL_HEADER_END


#endif /* __RPL__rpl_value__h__ */
