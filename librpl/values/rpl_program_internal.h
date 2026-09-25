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


struct rpl_program {
    /*! The values that make up the program. */
    rpl_adjbuffer_t _values;
};
typedef struct rpl_program rpl_program_t;


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


RPL_HEADER_END


#endif /* __RPL__rpl_program_internal__h__ */
