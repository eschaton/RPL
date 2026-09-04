//
//  rpl_complex_internal.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_complex_internal__h__
#define __RPL__rpl_complex_internal__h__

#include "rpl_complex.h"

#include <stdbool.h>

#include "rpl_real.h"


RPL_HEADER_BEGIN


struct rpl_complex {
    rpl_real_t _x;
    rpl_real_t _y;
};
typedef struct rpl_complex rpl_complex_t;

RPL_EXPORT
void
rpl_complex_free(rpl_value_t complex);

RPL_EXPORT
const char * RPL_NULLABLE
rpl_complex_copy_string(rpl_value_t complex,
			rpl_environment_t RPL_NULLABLE env);

RPL_EXPORT
const char * RPL_NULLABLE
rpl_complex_rep_copy_string(rpl_complex_t complex_rep,
			    rpl_environment_t RPL_NULLABLE env);

/*! Get the modulus (r) of a rectangular complex number. */
RPL_EXPORT
rpl_real_t
rpl_complex_rep_get_modulus(rpl_complex_t rectangular);

/*! Get the argument (θ) to a rectangular complex number. */
RPL_EXPORT
rpl_real_t
rpl_complex_rep_get_argument(rpl_complex_t rectangular);


RPL_HEADER_END


#endif /* __RPL__rpl_complex_internal__h__ */
