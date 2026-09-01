//
//  rpl_program_internal.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_program_internal__h__
#define __RPL__rpl_program_internal__h__

#include "rpl_program.h"

#include "rpl_value.h"


RPL_HEADER_BEGIN


struct rpl_program {
    // TODO: programs
};
typedef struct rpl_program rpl_program_t;


RPL_EXPORT
void
rpl_program_free(rpl_value_t program);


RPL_HEADER_END


#endif /* __RPL__rpl_program_internal__h__ */
