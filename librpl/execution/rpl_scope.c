//
//  rpl_scope.c
//  librpl
//
//  Created by Chris Hanson on 9/10/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_scope_internal.h"

#include <assert.h>
#include <stdlib.h>


RPL_SOURCE_BEGIN


rpl_scope_t RPL_NULLABLE
rpl_scope_new(rpl_scope_t RPL_NULLABLE parent)
{
    rpl_scope_t scope = calloc(1, sizeof(struct rpl_scope));
    if (scope) {
	scope->_parent = parent;

	bool initialized;
	initialized = rpl_adjbuffer_init(&scope->_names, 8,
					 sizeof(rpl_unistring_t));
	if (initialized == false) goto error;
	initialized = rpl_adjbuffer_init(&scope->_values, 8,
					 sizeof(rpl_value_t));
	if (initialized == false) goto error;
    }
    return scope;

error:
    rpl_scope_free(scope);
    return NULL;
}

void
rpl_scope_free(rpl_scope_t scope)
{
    assert(scope != NULL);

    rpl_adjbuffer_deinit(&scope->_names);
    rpl_adjbuffer_deinit(&scope->_values);

    free(scope);
}

rpl_scope_t RPL_NULLABLE
rpl_scope_get_parent(rpl_scope_t scope)
{
    return scope->_parent;
}

bool
rpl_scope_name_is_equal(rpl_adjbuffer_t *buffer, void *element0,
			void *element1, void * RPL_NULLABLE refcon)
{
    rpl_unistring_t str0 = element0;
    rpl_unistring_t str1 = element1;

    return rpl_unistring_compare(str0, str1) == 0;
}

ssize_t
rpl_scope_find_variable(rpl_scope_t scope, rpl_unistring_t name)
{
    assert(scope != NULL);
    assert(name != NULL);

    return rpl_adjbuffer_find(&scope->_names, &name,
			      rpl_scope_name_is_equal, NULL);
}

rpl_value_t RPL_NULLABLE
rpl_scope_get_variable(rpl_scope_t scope, rpl_unistring_t name)
{
    assert(scope != NULL);
    assert(name != NULL);

    rpl_value_t value = NULL;
    const ssize_t idx = rpl_scope_find_variable(scope, name);
    if (idx != -1) {
	value = rpl_adjbuffer_get(&scope->_values, idx);
    }

    return value;
}

bool
rpl_scope_set_variable(rpl_scope_t scope, rpl_unistring_t name,
		       rpl_value_t value)
{
    assert(scope != NULL);
    assert(name != NULL);
    assert(value != NULL);

    bool did_set = false;
    const ssize_t idx = rpl_scope_find_variable(scope, name);
    if (idx != -1) {
	rpl_adjbuffer_set(&scope->_values, idx, value);
	rpl_value_retain(value);
	did_set = true;
    } else {
	bool appended;
	appended = rpl_adjbuffer_append_element(&scope->_names, name);
	if (appended) {
	    rpl_unistring_retain(name);
	    appended = rpl_adjbuffer_append_element(&scope->_values,
						    value);
	    if (appended) {
		rpl_value_retain(value);
		did_set = true;
	    } else {
		const size_t name_idx
		    = rpl_adjbuffer_get_count(&scope->_names) - 1;
		rpl_adjbuffer_remove_element(&scope->_names, name_idx);
		rpl_unistring_release(name);
	    }
	}
    }

    return did_set;
}

void
rpl_scope_remove_variable(rpl_scope_t scope, rpl_unistring_t name)
{
    assert(scope != NULL);
    assert(name != NULL);

    rpl_value_t value = NULL;
    const ssize_t idx = rpl_scope_find_variable(scope, name);
    assert(idx != -1);

    value = rpl_adjbuffer_get(&scope->_values, idx);
    
    rpl_adjbuffer_remove_element(&scope->_names, idx);
    rpl_adjbuffer_remove_element(&scope->_values, idx);

    rpl_unistring_release(name);
    rpl_value_release(value);
}


RPL_SOURCE_END
