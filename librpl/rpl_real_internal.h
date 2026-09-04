//
//  rpl_real_internal.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_real_internal__h__
#define __RPL__rpl_real_internal__h__

#include "rpl_real.h"


RPL_HEADER_BEGIN


RPL_EXPORT
void
rpl_real_free(rpl_value_t real);

RPL_EXPORT
const char * RPL_NULLABLE
rpl_real_copy_string(rpl_value_t real,
		     rpl_environment_t RPL_NULLABLE env);

RPL_EXPORT
const char * RPL_NULLABLE
rpl_real_rep_copy_string(rpl_real_t real_rep,
			 rpl_environment_t RPL_NULLABLE env);

/*!
 Create a string from a real that represents an angle in radians, taking
 the passed environment's angle mode into account.
 */
RPL_EXPORT
const char * RPL_NULLABLE
rpl_real_rep_copy_angle_string(rpl_real_t real_rep,
			       rpl_environment_t RPL_NULLABLE env);


RPL_HEADER_END


#endif /* __RPL__rpl_real_internal__h__ */
