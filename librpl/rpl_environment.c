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
	env->_angle_mode = rpl_angle_mode_degrees;
	env->_coordinate_system = rpl_coordinate_system_rectangular;
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

rpl_angle_mode_t
rpl_environment_get_angle_mode(rpl_environment_t env)
{
    assert(env != NULL);

    return env->_angle_mode;
}

void
rpl_environment_set_angle_mode(rpl_environment_t env,
			       rpl_angle_mode_t angle_mode)
{
    assert(env != NULL);

    env->_angle_mode = angle_mode;
}

rpl_coordinate_system_t
rpl_environment_get_coordinate_system(rpl_environment_t env)
{
    assert(env != NULL);

    return env->_coordinate_system;
}

void
rpl_environment_set_coordinate_system(rpl_environment_t env,
				      rpl_coordinate_system_t cs)
{
    assert(env != NULL);

    env->_coordinate_system = cs;
}


RPL_SOURCE_END
