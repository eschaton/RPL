//
//  rpl_token_internal.h
//  librpl
//
//  Created by Chris Hanson on 9/4/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_token_internal__h__
#define __RPL__rpl_token_internal__h__

#include "rpl_token.h"


RPL_HEADER_BEGIN


struct rpl_token {
    rpl_token_type_t _type;
    rpl_unistring_t RPL_NULLABLE _str;
    rpl_value_t RPL_NULLABLE _value;
};

RPL_EXPORT
rpl_token_t RPL_NULLABLE
rpl_token_new(rpl_token_type_t _type,
	      rpl_unistring_t RPL_NULLABLE _str,
	      rpl_value_t RPL_NULLABLE _value);


RPL_HEADER_END


#endif /* __RPL__rpl_token_internal__h__ */
