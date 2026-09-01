//
//  rpl_tagged.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_tagged_internal.h"

#include <assert.h>

#include "rpl_value_internal.h"


RPL_SOURCE_BEGIN


rpl_value_t RPL_NULLABLE
rpl_tagged_new(rpl_value_t tag, rpl_value_t value)
{
    assert(tag != NULL);
    assert(tag->_type == rpl_type_name);
    assert(value != NULL);

    rpl_value_t tagged = rpl_value_new(rpl_type_tagged);
    if (tagged) {
	tagged->_reps._tagged._tag = rpl_value_retain(tag);
	tagged->_reps._tagged._value = rpl_value_retain(value);
    }
    return tagged;
}

void
rpl_tagged_free(rpl_value_t tagged)
{
    assert(tagged != NULL);
    assert(tagged->_type == rpl_type_tagged);

    rpl_value_release(tagged->_reps._tagged._tag);
    rpl_value_release(tagged->_reps._tagged._value);
}

rpl_value_t
rpl_tagged_get_tag(rpl_value_t tagged)
{
    assert(tagged != NULL);
    assert(tagged->_type == rpl_type_tagged);

    return tagged->_reps._tagged._tag;
}

rpl_value_t
rpl_tagged_get_value(rpl_value_t tagged)
{
    assert(tagged != NULL);
    assert(tagged->_type == rpl_type_tagged);

    return tagged->_reps._tagged._value;
}


RPL_SOURCE_END
