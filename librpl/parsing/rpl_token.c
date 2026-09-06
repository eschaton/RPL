//
//  rpl_token.c
//  librpl
//
//  Created by Chris Hanson on 9/4/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_token_internal.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


RPL_SOURCE_BEGIN


rpl_token_t RPL_NULLABLE
rpl_token_new(rpl_token_type_t type,
	      const char * RPL_NULLABLE str,
	      rpl_value_t RPL_NULLABLE value)
{
    assert((type == rpl_token_type_identifier) && (str != NULL));
    assert((type != rpl_token_type_value)
	   || ((type == rpl_token_type_value) && (value != NULL)));

    rpl_token_t token = calloc(1, sizeof(struct rpl_token));
    if (token) {
	token->_type = type;
	if (type == rpl_token_type_identifier) {
	    token->_str = strdup(str);
	    if (token->_str) goto error;
	}
	if (type == rpl_token_type_value && (value != NULL)) {
	    token->_value = rpl_value_retain(value);
	}
    }

    return token;

error:
    rpl_token_free(token);
    return NULL;
}

void
rpl_token_free(rpl_token_t token)
{
    assert(token != NULL);

    free((void *)token->_str);

    if ((token->_type == rpl_token_type_value)
	&& (token->_value != NULL))
    {
	rpl_value_release(token->_value);
    }

    free(token);
}

rpl_token_type_t
rpl_token_get_type(rpl_token_t token)
{
    assert(token != NULL);

    return token->_type;
}

const char *
rpl_token_get_str(rpl_token_t token)
{
    assert(token != NULL);

    return token->_str;
}

rpl_value_t RPL_NULLABLE
rpl_token_get_value(rpl_token_t token)
{
    assert(token != NULL);

    return token->_value;
}


RPL_SOURCE_END
