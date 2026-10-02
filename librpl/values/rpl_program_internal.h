//
//  rpl_program_internal.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_program_internal__h__
#define __RPL__rpl_program_internal__h__

#include "rpl_program.h"

#include "rpl_adjbuffer.h"


RPL_HEADER_BEGIN


/*!
 The type of a program.

 Programs are both user-authored and created as a side-effect of parsing
 control-flow constructs. A program's type distinguishes these.

 - NOTE: These are entirely internal to the implementation, there is no
         support for user-defined control flow constructs.
 */
typedef enum rpl_program_type {
    /*! A generic user-authored program. */
    rpl_program_type_generic = 0,

    /*! An intermediate program within a control-flow construct. */
    rpl_program_type_intermediate,

    /*! A program representing a `DO ... UNTIL ... END`. */
    rpl_program_type_DO,

    /*! A program representing an `IF ... THEN ... [ELSE ...] END`. */
    rpl_program_type_IF,

    /*! A program representing a `FOR ... NEXT|STEP`. */
    rpl_program_type_FOR,

    /*! A program representing a `CASE {... THEN ... END} [...] END`. */
    rpl_program_type_CASE,

    /*! A program representing a `START ... NEXT|STEP`. */
    rpl_program_type_START,

    /*! A program representing a `WHILE ... REPEAT ... END`. */
    rpl_program_type_WHILE,
} rpl_program_type_t;


struct rpl_program {
    /*! The type of the program. */
    rpl_program_type_t _type;

    /*! The values that make up the program. */
    rpl_adjbuffer_t _values;
};
typedef struct rpl_program rpl_program_t;


/*! Create a program of a specific type. */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_program_new_with_type(rpl_program_type_t type);

/*! Free resources held by the program. */
RPL_EXPORT
void
rpl_program_free(rpl_value_t program);

/*! Copy a string representation of the program. */
RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_program_copy_string(rpl_value_t program,
			rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED;

/*! Get the program's type. */
RPL_EXPORT
rpl_program_type_t
rpl_program_get_type(rpl_value_t program);

RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_program_generic_copy_string(rpl_value_t program,
				rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED;

RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_program_intermediate_copy_string(rpl_value_t program,
				     rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED;

RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_program_DO_copy_string(rpl_value_t program,
			   rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED;

RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_program_IF_copy_string(rpl_value_t program,
			   rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED;

RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_program_FOR_copy_string(rpl_value_t program,
			    rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED;

RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_program_CASE_copy_string(rpl_value_t program,
			     rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED;

RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_program_START_copy_string(rpl_value_t program,
			      rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED;

RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_program_WHILE_copy_string(rpl_value_t program,
			      rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED;


RPL_HEADER_END


#endif /* __RPL__rpl_program_internal__h__ */
