//
//  rpl_complex_internal.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_complex.h"

#include <stdbool.h>

#include "rpl_real.h"


#ifndef __RPL__rpl_complex_internal__h__
#define __RPL__rpl_complex_internal__h__


RPL_HEADER_BEGIN


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


RPL_HEADER_END


#endif /* __RPL__rpl_complex_internal__h__ */
