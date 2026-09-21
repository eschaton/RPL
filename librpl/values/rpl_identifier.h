//
//  rpl_identifier.h
//  librpl
//
//  Created by Chris Hanson on 9/21/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_identifier__h__
#define __RPL__rpl_identifier__h__

#include "rpl_value.h"


RPL_HEADER_BEGIN


/*
 An identifier is a canonicalized string representing either a command
 or a variable access. It's a type of value that's internal to the
 interpreter; all API should be in terms of either names or unistring.
 */


RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_identifier_new(rpl_unistring_t str);

RPL_EXPORT
rpl_unistring_t
rpl_name_get_rep(rpl_value_t val);


RPL_HEADER_END


#endif /* __RPL__rpl_identifier__h__ */
