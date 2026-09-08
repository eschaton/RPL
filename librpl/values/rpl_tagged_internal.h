//
//  rpl_tagged_internal.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_tagged_internal__h__
#define __RPL__rpl_tagged_internal__h__

#include "rpl_tagged.h"

#include "rpl_real.h"


RPL_HEADER_BEGIN


struct rpl_tagged {
    /*! The tag, which must be a name. */
    rpl_value_t _tag;

    /*! The value, which can be anything. */
    rpl_value_t _value;
};
typedef struct rpl_tagged rpl_tagged_t;

RPL_EXPORT
void
rpl_tagged_free(rpl_value_t val);

RPL_EXPORT
rpl_unistring_t RPL_NULLABLE
rpl_tagged_copy_string(rpl_value_t tagged,
		       rpl_environment_t RPL_NULLABLE env);


RPL_HEADER_END


#endif /* __RPL__rpl_tagged_internal__h__ */
