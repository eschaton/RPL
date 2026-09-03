//
//  rpl_environment.c
//  librpl
//
//  Created by Chris Hanson on 9/2/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_environment_internal.h"

#include <assert.h>
#include <stdlib.h>


RPL_SOURCE_BEGIN


rpl_environment_t RPL_NULLABLE
rpl_environment_new(void)
{
    rpl_environment_t env = calloc(1, sizeof(struct rpl_environment));
    if (env) {
	env->_base = rpl_base_decimal;
    }
    return env;
}

void
rpl_environment_free(rpl_environment_t env)
{
    assert(env != NULL);

    free(env);
}

rpl_base_t
rpl_environment_get_base(rpl_environment_t env)
{
    assert(env != NULL);

    return env->_base;
}

void
rpl_environment_set_base(rpl_environment_t env, rpl_base_t base)
{
    assert(env != NULL);

    env->_base = base;
}


RPL_SOURCE_END
