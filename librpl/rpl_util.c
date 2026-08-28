//
//  rpl_util.c
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_util.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


RPL_SOURCE_BEGIN


void *
rpl_util_carray_copy(void *carray, size_t carray_count,
		     size_t elem_size)
{
    assert(carray != NULL);
    assert(carray_count > 0);
    assert(elem_size > 0);

    const size_t size = carray_count * elem_size;
    void *result = malloc(size);
    if (result) {
	memcpy(result, carray, size);
    }

    return result;
}

rpl_integer_t
rpl_util_carray_integer_multily(const rpl_integer_t * const carray,
				rpl_integer_t carray_count)
{
    assert(carray != NULL);
    assert(carray_count > 0);

    rpl_integer_t result = 1;
    for (size_t i = 0; i < carray_count; i++) {
	result *= carray[i];
    }

    return result;
}


RPL_SOURCE_END
