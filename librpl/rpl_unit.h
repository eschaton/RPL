//
//  rpl_unit.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_defines.h"

#include "rpl_real.h"
#include "rpl_value.h"


#ifndef __RPL__rpl_unit__h__
#define __RPL__rpl_unit__h__


RPL_HEADER_BEGIN


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


#endif /* __RPL__rpl_unit__h__ */
