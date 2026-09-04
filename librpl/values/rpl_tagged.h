//
//  rpl_tagged.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_tagged__h__
#define __RPL__rpl_tagged__h__

#include "rpl_defines.h"

#include "rpl_value.h"


RPL_HEADER_BEGIN


RPL_EXPORT
rpl_value_t RPL_NULLABLE
rpl_tagged_new(rpl_value_t tag, rpl_value_t value);

RPL_EXPORT
rpl_value_t
rpl_tagged_get_tag(rpl_value_t tagged);

RPL_EXPORT
rpl_value_t
rpl_tagged_get_value(rpl_value_t tagged);


RPL_HEADER_END


#endif /* __RPL__rpl_tagged__h__ */
