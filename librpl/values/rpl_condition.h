//
//  rpl_condition.h
//  librpl
//
//  Created by Chris Hanson on 10/9/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_condition__h__
#define __RPL__rpl_condition__h__

#include "rpl_value.h"

#include <stdarg.h>

#include "rpl_integer.h"


RPL_HEADER_BEGIN


/*!
 Returns a newly-allocated condition with a given @a name.

 Additional payload can be supplied in the form of key-value pairs,
 terminated with `NULL`.
 */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_condition_new_with_name(rpl_unistring_t name, ...);

/*! Gets the name of a condition. */
RPL_EXPORT
rpl_unistring_t
rpl_condition_get_name(rpl_value_t condition);

/*! Gets a value from a condition's payload, `NULL` if not present. */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_condition_get_payload_value(rpl_value_t condition, rpl_value_t key);


/* MARK: - Signaling & Handling Conditions */

/*! Signal the given condition. */
RPL_EXPORT
void
rpl_condition_signal(rpl_value_t condition);

/*! Obtain the most recently signaled condition. */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_condition_latest_signaled(void)
RPL_RETURNS_RETAINED;


/* MARK: - Standard Conditions */

/*! "Out of memory" condition (static). */
RPL_EXPORT
rpl_value_t
rpl_condition_memory_full(void)
RPL_RETURNS_RETAINED;

/*! "Insufficient arguments" condition (dynamic). */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_condition_insufficient_arguments(rpl_value_t identifier,
				     rpl_integer_t expected,
				     rpl_integer_t actual)
RPL_RETURNS_RETAINED;

/*! "Invalid type" condition (dynamic). */
RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_condition_invalid_type(rpl_value_t identifier,
			   rpl_type_t expected,
			   rpl_type_t actual)
RPL_RETURNS_RETAINED;


RPL_HEADER_END


#endif /* __RPL__rpl_condition__h__ */
