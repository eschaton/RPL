//
//  rpl_interpreter_defaults.c
//  librpl
//
//  Created by Chris Hanson on 9/12/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_interpreter.h"

#include <assert.h>
#include <math.h>
#include <string.h>

#include "rpl_context.h"
#include "rpl_operations.h"

#include "rpl_integer.h"
#include "rpl_real.h"


RPL_SOURCE_BEGIN


bool
rpl_constants_configure(rpl_scope_t scope);


bool
rpl_configure_defaults(rpl_interpreter_t interp,
		       void * RPL_NULLABLE refcon)
{
    assert(interp != NULL);

    bool success = true;

    rpl_context_t context = NULL;
    rpl_operation_table_t table = NULL;
    rpl_scope_t constant_scope = NULL;

    context = rpl_interpreter_get_context(interp);
    assert(context != NULL);

    table = rpl_interpreter_get_operation_table(interp);
    assert(table != NULL);

    constant_scope = rpl_context_get_constant_scope(context);
    assert(constant_scope != NULL);

    success = success && rpl_operations_configure(table);
    success = success && rpl_constants_configure(constant_scope);

    return true;
}


struct rpl_constant_entry {
    const char *_name;
    rpl_type_t _type;
    rpl_integer_t _integer_val;
    rpl_real_t _real_val;
} constants[] = {
    { "π", rpl_type_real, 0, M_PI },
    { "e", rpl_type_real, 0, M_E },
    { NULL },
};

bool
rpl_constants_configure(rpl_scope_t scope)
{
    assert(scope != NULL);

    rpl_unistring_t name = NULL;
    rpl_value_t value = NULL;

    for (struct rpl_constant_entry *entry = &constants[0];
	 entry->_name != NULL;
	 entry++)
    {
	const size_t name_len = strlen(entry->_name);
	name = rpl_unistring_new_from_utf8(entry->_name, name_len);
	if (name == NULL) goto error;

	switch (entry->_type) {
	    case rpl_type_integer: {
		value = rpl_integer_new(entry->_integer_val);
		if (value == NULL) goto error;
	    } break;

	    case rpl_type_real: {
		value = rpl_real_new(entry->_real_val);
		if (value == NULL) goto error;
	    } break;

	    default: {
		/* TODO: Support more constant types. */
		assert((entry->_type == rpl_type_integer)
		       || (entry->_type == rpl_type_real));
	    } break;
	}

	bool did_set = rpl_scope_set_variable(scope, name, value);
	if (did_set == false) goto error;

	rpl_unistring_release(name);
	rpl_value_release(value);
    }

    return true;

error:
    if (name) rpl_unistring_release(name);
    if (value) rpl_value_release(value);
    return false;
}


RPL_SOURCE_END
