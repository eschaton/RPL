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
    rpl_type_t _type;
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


RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_value_new(rpl_type_t type);


RPL_EXPORT
void
rpl_value_free_array(rpl_value_t RPL_NONNULL * RPL_NONNULL vals,
		     rpl_integer_t vals_count);



RPL_HEADER_END


#endif /* __RPL__rpl_value_internal__h__ */
