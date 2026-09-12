//
//  rpl_unit_internal.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_unit_internal__h__
#define __RPL__rpl_unit_internal__h__

#include "rpl_unit.h"

#include "rpl_real.h"
#include "rpl_value.h"


RPL_HEADER_BEGIN


struct rpl_unit {
    /*! The scalar portion of a unit. */
    rpl_real_t _scalar;

    /*! The unit descriptor, which must be an `rpl_type_name`. */
    rpl_value_t _units;
};
typedef struct rpl_unit rpl_unit_t;

RPL_EXPORT
void
rpl_unit_free(rpl_value_t val);

RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_unit_copy_string(rpl_value_t unit,
		     rpl_environment_t RPL_NULLABLE env)
RPL_RETURNS_RETAINED;


RPL_HEADER_END


#endif /* __RPL__rpl_unit_internal__h__ */
