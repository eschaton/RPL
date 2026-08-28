//
//  rpl_value_internal.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_value_internal__h__
#define __RPL__rpl_value_internal__h__

#include "rpl_value.h"


RPL_HEADER_BEGIN


/* MARK: - Integer Numbers */

RPL_EXPORT
void
rpl_integer_free(rpl_value_t integer);


/* MARK: - Real Numbers */

RPL_EXPORT
void
rpl_real_free(rpl_value_t real);


/* MARK: - Complex Numbers */

struct rpl_complex {
    bool _is_polar;
    union {
	struct {
	    rpl_real_t _x;
	    rpl_real_t _y;
	} _rectangular;
	struct {
	    rpl_real_t _r;
	    rpl_real_t _theta;
	} _polar;
    } _value;
};
typedef struct rpl_complex rpl_complex_t;

RPL_EXPORT
void
rpl_complex_free(rpl_value_t complex);


/* MARK: - Arrays */

struct rpl_array {
    rpl_integer_t _dims_count;
    rpl_integer_t *_dims;
    rpl_integer_t _vals_count;
    rpl_value_t RPL_NONNULL * RPL_NONNULL _vals;
};
typedef struct rpl_array rpl_array_t;

RPL_EXPORT
void
rpl_array_free(rpl_value_t array);


/* MARK: - Name */

struct rpl_name {
    rpl_integer_t _chars_len;
    char *_chars;
};
typedef struct rpl_name rpl_name_t;

RPL_EXPORT
void
rpl_name_free(rpl_value_t name);


/* MARK: - Program */

struct rpl_program {
    // TODO: programs
};
typedef struct rpl_program rpl_program_t;

RPL_EXPORT
void
rpl_program_free(rpl_value_t program);


/* MARK: - String */

struct rpl_string {
    char *_chars;
    rpl_integer_t _chars_len;
};
typedef struct rpl_string rpl_string_t;

RPL_EXPORT
void
rpl_string_free(rpl_value_t string);


/* MARK: - List */

struct rpl_list {
    /*! The first value in the pair. */
    rpl_value_t RPL_NULLABLE _car;

    /*! The second value in the pair. */
    rpl_value_t RPL_NULLABLE _cdr;
};
typedef struct rpl_list rpl_list_t;

void
rpl_list_free(rpl_value_t val);


/* MARK: - Tagged */

struct rpl_tagged {
    /*! The tag, which must be a name. */
    rpl_value_t _tag;

    /*! The value, which can be anything. */
    rpl_value_t _value;
};
typedef struct rpl_tagged rpl_tagged_t;

void
rpl_tagged_free(rpl_value_t val);


/* MARK: - Unit */

struct rpl_unit {
    /*! The scalar portion of a unit. */
    rpl_real_t _scalar;

    /*! The unit descriptor, which must be an `rpl_type_name`. */
    rpl_value_t _units;
};
typedef struct rpl_unit rpl_unit_t;

void
rpl_unit_free(rpl_value_t val);


/* MARK: - Generic Value Behavior */

struct rpl_value {
    rpl_type_t _type;
    union {
	rpl_integer_t _integer;
	rpl_real_t _real;
	rpl_complex_t _complex;
	rpl_array_t _array;
	rpl_name_t _name;
	rpl_program_t _program;
	rpl_string_t _string;
	rpl_list_t _list;
	rpl_tagged_t _tagged;
	rpl_unit_t _unit;
    } _reps;
};


RPL_HEADER_END


#endif /* __RPL__rpl_value_internal__h__ */
