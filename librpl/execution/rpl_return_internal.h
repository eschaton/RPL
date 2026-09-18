//
//  rpl_return_internal.h
//  librpl
//
//  Created by Chris Hanson on 9/18/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_return_internal__h__
#define __RPL__rpl_return_internal__h__

#include "rpl_return.h"

#include "rpl_adjbuffer.h"


RPL_HEADER_BEGIN


/*!
 Each item in the return has both a return operation and a level, though
 the interpretation and handling of these is up to the first return
 operation in the return. (As an example, `IF_ret` may simply use the
 `THEN_ret` and `ELSE_ret` return operations as markers, rather than
 invoke them.)
 */
typedef struct rpl_return_entry {
    rpl_return_operation_t _operation;
    rpl_integer_t _level;
} rpl_return_entry_t;


struct rpl_return {
    /*!
     The entries within the return; while it's tempting to make this a
     fixed size, something like a `CASE` construct requires the ability
     to specify an arbitrary number of entries.
     */
    rpl_adjbuffer_t _entries;

    /*! Whether the return is closed. */
    bool _is_closed;
};


RPL_HEADER_END


#endif /* __RPL__rpl_return_internal__h__ */
