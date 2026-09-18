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

#include "rpl_unistring_internal.h"
#include "rpl_value_internal.h"


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

	scope->_is_mutable = true;
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
    assert(buffer != NULL);
    assert(element0 != NULL);
    assert(element1 != NULL);

    rpl_unistring_t *str0 = element0;
    rpl_unistring_t *str1 = element1;

    return rpl_unistring_is_equal_case_insensitive(*str0, *str1);
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
rpl_scope_get_variable(rpl_scope_t scope, rpl_unistring_t name,
		       bool search_parents)
{
    assert(scope != NULL);
    assert(name != NULL);

    rpl_value_t value = NULL;
    ssize_t idx = -1;

    /*
     Starting with the passed scope, search up the scope chain (if the
     caller requested it) until a value for the variable is found.
     */

    for (rpl_scope_t s = scope;
	 search_parents && (s != NULL) && (idx == -1);
	 s = s->_parent)
    {
	idx = rpl_scope_find_variable(s, name);
	if (idx != -1) {
	    rpl_value_t *value_p = rpl_adjbuffer_get(&s->_values, idx);
	    assert(value_p != NULL);
	    value = *value_p;
	}
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
    assert(scope->_is_mutable);

    bool did_set = false;

    /*
     Find the variable in the scope if it's already present and, if so,
     replace it there. Otherwise append it.
     */

    const ssize_t idx = rpl_scope_find_variable(scope, name);
    if (idx != -1) {
	rpl_value_retain(value);
	rpl_adjbuffer_set(&scope->_values, idx, &value);
	did_set = true;
    } else {
	/*
	 Neither name nor value is leaked, because the scope takes
	 ownership of them but that can't be expressed except by using
	 the "rpl_…_not_leaked()" functions.
	*/

	bool appended_name
	    = rpl_adjbuffer_append_element(&scope->_names, &name);
	if (appended_name) {
	    rpl_unistring_not_leaked(rpl_unistring_retain(name));
	    bool appended_value
		= rpl_adjbuffer_append_element(&scope->_values, &value);
	    if (appended_value) {
		rpl_value_not_leaked(rpl_value_retain(value));
		did_set = true;
	    } else {
		/* If appending a value failed, un-append the name. */
		const size_t name_idx
		    = rpl_adjbuffer_get_count(&scope->_names) - 1;
		rpl_adjbuffer_remove_element(&scope->_names, name_idx);
		rpl_unistring_release(name);
		did_set = false;
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
    assert(scope->_is_mutable);

    rpl_value_t *pvalue = NULL;
    const ssize_t idx = rpl_scope_find_variable(scope, name);
    assert(idx != -1);

    pvalue = rpl_adjbuffer_get(&scope->_values, idx);

    rpl_adjbuffer_remove_element(&scope->_names, idx);
    rpl_adjbuffer_remove_element(&scope->_values, idx);

    rpl_unistring_release(name);
    rpl_value_release(*pvalue);
}

void
rpl_scope_make_immutable(rpl_scope_t scope)
{
    assert(scope != NULL);
    assert(scope->_is_mutable);

    scope->_is_mutable = false;
}


RPL_SOURCE_END
