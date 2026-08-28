//
//  rpl_complex.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_defines.h"

#include "rpl_real.h"
#include "rpl_value.h"


#ifndef __RPL__rpl_complex__h__
#define __RPL__rpl_complex__h__


RPL_HEADER_BEGIN


/*! Create a new rectangular complex value. */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_complex_rectangular_new(rpl_real_t x_rep, rpl_real_t y_rep);

/*! Create a new polar complex value. */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_complex_polar_new(rpl_real_t r_rep, rpl_real_t theta_rep);

// TODO: Complex Number Operations


RPL_HEADER_END


#endif /* __RPL__rpl_complex__h__ */
