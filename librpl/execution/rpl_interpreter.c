//
//  rpl_interpreter.c
//  librpl
//
//  Created by Chris Hanson on 9/10/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "rpl_interpreter_internal.h"

#include <assert.h>
#include <stdlib.h>

#include "rpl_context.h"
#include "rpl_scope.h"
#include "rpl_tokenizer.h"


RPL_SOURCE_BEGIN


rpl_interpreter_t RPL_NULLABLE
rpl_interpreter_new(void)
{
    rpl_interpreter_t interp
	= calloc(1, sizeof(struct rpl_interpreter));
    if (interp) {
	interp->_context = rpl_context_new();
	if (interp->_context == NULL) goto error;

	interp->_tokenizer = rpl_tokenizer_new(interp->_context);
	if (interp->_tokenizer == NULL) goto error;

	interp->_global = rpl_scope_new(interp->_local);
	if (interp->_global == NULL) goto error;

	interp->_local = rpl_scope_new(interp->_global);
	if (interp->_local == NULL) goto error;

	interp->_output = rpl_unistring_new(80 * 24);
	if (interp->_output == NULL) goto error;
    }
    return interp;

error:
    rpl_interpreter_free(interp);
    return NULL;
}

RPL_EXPORT
void
rpl_interpreter_free(rpl_interpreter_t interp)
{
    assert(interp != NULL);

    if (interp->_context) rpl_context_free(interp->_context);
    if (interp->_tokenizer) rpl_tokenizer_free(interp->_tokenizer);
    if (interp->_global) rpl_scope_free(interp->_global);
    if (interp->_local) rpl_scope_free(interp->_local);

    free(interp);
}

RPL_EXPORT
bool
rpl_interpreter_append_input(rpl_interpreter_t interp,
			     rpl_unistring_t str)
{
    assert(interp != NULL);
    assert(str != NULL);

    return rpl_tokenizer_append(interp->_tokenizer, str);
}

RPL_EXPORT
bool
rpl_interpreter_has_output(rpl_interpreter_t interp)
{
    assert(interp != NULL);

    return rpl_unistring_get_length(interp->_output) > 0;
}

rpl_unistring_t RPL_NULLABLE
rpl_interpreter_copy_output(rpl_interpreter_t interp)
{
    assert(interp != NULL);

    rpl_unistring_t output = rpl_unistring_copy(interp->_output);
    if (output) {
	rpl_unistring_remove_all(interp->_output);
    }

    return output;
}


RPL_SOURCE_END
