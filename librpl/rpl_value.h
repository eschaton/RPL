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


/*! The types of values RPL supports. */
enum rpl_type {
    /*! A binary integer is an unsigned 64-bit value. */
    rpl_type_integer,
    
    /*!
     A real number is represented as a 64-bit IEEE-754 double-precision
     floataing point value.
     */
    rpl_type_real,
    
    /*!
     A complex number represents `a+b*i` as a pair of real numbers.
     
     - NOTE: The type imposes no constraints on the meaning of `a` and
             `b`; that is, it does not distinguish rectangular and polar
	     complex numbers.
     */
    rpl_type_complex,
    
    /*!
     An array in RPL is a one-dimensional 0-indexed mutable collection
     whose contents can be real, complex, or array values and are
     represented efficiently.
     
     - NOTE: When one array contains another, the containing array just
             holds a reference to it, it is not copied.
     */
    rpl_type_array,
    
    rpl_type_name,
    rpl_type_program,
    rpl_type_string,
    rpl_type_list,
    rpl_type_tagged,
    rpl_type_unit,
};
typedef enum rpl_type rpl_type_t;


/*! A typed value in RPL. */
typedef struct rpl_value *rpl_value_t;


/*! Get the type of a value. */
RPL_EXPORT
rpl_type_t
rpl_value_get_type(rpl_value_t val);

/*! Retain a value. */
RPL_EXPORT
rpl_value_t
rpl_value_retain(rpl_value_t val);

/*! Release a value. */
RPL_EXPORT
void
rpl_value_release(rpl_value_t val);

/*!
 Copy a value.

 - NOTE: Immutable values will just be retained, rather than have a new
	 instance created, because nothing should rely on them being
	 distinct. Mutable values are copied deeply.
 */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_value_copy(rpl_value_t val);

/*!
 Copy the string representation of a value.

 - NOTE: The memory for the string must be released with ``free(3``.
 */
RPL_EXPORT
const char * RPL_NULLABLE
rpl_value_copy_string(rpl_value_t val);


RPL_HEADER_END


#endif /* __RPL__rpl_value__h__ */
