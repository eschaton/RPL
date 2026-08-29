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

#include "rpl_integer_internal.h"
#include "rpl_real_internal.h"
#include "rpl_complex_internal.h"
#include "rpl_array_internal.h"
#include "rpl_name_internal.h"
#include "rpl_program_internal.h"
#include "rpl_string_internal.h"
#include "rpl_list_internal.h"
#include "rpl_tagged_internal.h"
#include "rpl_unit_internal.h"


RPL_HEADER_BEGIN


struct rpl_value {
    /*! Type of the value. */
    rpl_type_t _type;

    /*!
     Number of references to the value.

     This starts at `1` and when it goes to `0`, the value is freed. As
     a special case, `RPL_INTEGER_MAX` is a sentinel used to represent a
     "permanent" value.
     */
    rpl_integer_t _refs;

    /*!
     The various underlying representations of a value, selected
     according to the value of `_type`.
     */
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


/*! Allocate a new value of the given type. */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_value_new(rpl_type_t type);


/*! Free an allocated value. */
RPL_EXPORT
void
rpl_value_free(rpl_value_t val);


/*! Make a value immortal (immune from retain/release). */
RPL_EXPORT
void
rpl_value_immortalize(rpl_value_t val);


/*!
 Release every value in a C array of values.

 - NOTE: Does not ``free(3)`` the C array itself.
 */
RPL_EXPORT
void
rpl_value_release_array(rpl_value_t RPL_NONNULL * RPL_NONNULL vals,
			rpl_integer_t vals_count);



RPL_HEADER_END


#endif /* __RPL__rpl_value_internal__h__ */
