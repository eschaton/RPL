//
//  rpl_unistring_internal.h
//  librpl
//
//  Created by Chris Hanson on 9/8/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_unistring_internal__h__
#define __RPL__rpl_unistring_internal__h__

#include "rpl_unistring.h"


RPL_HEADER_BEGIN


struct rpl_unistring {
    rpl_unichar_t *_storage;
    size_t _count;
    size_t _capacity;
    int _refcnt;
};


RPL_EXPORT
void
rpl_unistring_free(rpl_unistring_t str);


RPL_HEADER_END


#endif /* __RPL__rpl_unistring_internal__h__ */
