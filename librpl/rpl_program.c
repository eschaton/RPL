//
//  rpl_program.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_program_internal.h"

#include <assert.h>
#include <stdlib.h>

#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN


void
rpl_program_free(rpl_value_t program)
{
    assert(program != NULL);
    assert(program->_type == rpl_type_program);

    // TODO: Free program
}

// TODO: Program Operations


RPL_SOURCE_END
