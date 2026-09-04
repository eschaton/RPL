//
//  rpl_environment_internal.h
//  librpl
//
//  Created by Chris Hanson on 9/2/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_environment_internal__h__
#define __RPL__rpl_environment_internal__h__

#include "rpl_environment.h"


RPL_HEADER_BEGIN


struct rpl_environment {
    rpl_base_t _base;
    rpl_angle_mode_t _angle_mode;
    rpl_coordinate_system_t _coordinate_system;
};


RPL_HEADER_END


#endif /* __RPL__rpl_environment_internal__h__ */
