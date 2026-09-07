//
//  rpl_strbuffer_internal.h
//  RPL
//
//  Created by Chris Hanson on 9/4/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_strbuffer_internal__h__
#define __RPL__rpl_strbuffer_internal__h__

#include "rpl_strbuffer.h"

#include "rpl_adjbuffer.h"


RPL_HEADER_BEGIN


struct rpl_strbuffer {
    char *_storage;
    size_t _count;
    size_t _capacity;
};


RPL_HEADER_END


#endif /* __RPL__rpl_strbuffer_internal__h__ */
