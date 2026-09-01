//
//  rpl_complex.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_complex__h__
#define __RPL__rpl_complex__h__

#include "rpl_defines.h"

#include "rpl_real.h"
#include "rpl_value.h"


RPL_HEADER_BEGIN


/*! Create a new complex value. */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_complex_new(rpl_real_t a_rep, rpl_real_t b_rep);

// TODO: Complex Number Operations


RPL_HEADER_END


#endif /* __RPL__rpl_complex__h__ */
