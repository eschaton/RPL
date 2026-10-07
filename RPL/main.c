//
//  main.c
//  RPL
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include <assert.h>
#include <errno.h>
#include <histedit.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>

#include "rpl.h"


RPL_SOURCE_BEGIN


EditLine *RPL_editor = NULL;
History *RPL_history = NULL;
rpl_interpreter_t RPL_interpreter = NULL;


/*! Produce a prompt for the command line. */
char * RPL_NONNULL
RPL_prompt(EditLine *editor)
{
    return "RPL> ";
}

/*!
 Send any output from an interpreter along to `stdout`.

 - Returns: 0 on success, -`errno` on failure
 */
int
RPL_send_along_output(rpl_interpreter_t interpreter)
{
    int saved_errno = 0;

    rpl_unistring_t output_str = NULL;
    const char *output_utf8 = NULL;

    if (rpl_interpreter_has_output(RPL_interpreter)) {
	output_str = rpl_interpreter_copy_output(interpreter);
	if (output_str == NULL) {
	    saved_errno = ENOMEM;
	    goto error;
	}

	output_utf8 = rpl_unistring_copy_utf8(output_str);
	if (output_utf8 == NULL) {
	    saved_errno = ENOMEM;
	    goto error;
	}

	fprintf(stdout, "%s", output_utf8);

	free((void *)output_utf8);
	rpl_unistring_release(output_str);
    }

    return 0;

error:
    return -saved_errno;
}

int
main(int argc, const char * RPL_NULLABLE argv[RPL_NONNULL])
{
    int saved_errno = 0;

    RPL_editor = el_init("RPL", stdin, stdout, stderr);
    if (RPL_editor == NULL) {
	fprintf(stderr, "RPL: error: Couldn't initialize editline\n");
	goto error;
    }

    RPL_history = history_init();
    if (RPL_history == NULL) {
	fprintf(stderr, "RPL: error: Couldn't initialize history\n");
	goto error;
    }

    el_set(RPL_editor, EL_EDITOR, "emacs");
    el_set(RPL_editor, EL_HIST, history, RPL_history);
    el_set(RPL_editor, EL_PROMPT, RPL_prompt);
    el_set(RPL_editor, EL_SIGNAL, 1);

#if __APPLE__
    /*
     If running under Xcode's debugger, "TERM=dumb" can be set even when
     I/O is handled using Terminal.app rather than the debugger UI. In
     that case, specify the terminal is a VT100, otherwise pull from the
     environment.
     */
    if (getenv("__XCODE_BUILT_PRODUCTS_DIR_PATHS") != NULL) {
	const char *TERM = getenv("TERM");
	if (TERM && (strcmp(TERM, "dumb") == 0)) {
	    el_set(RPL_editor, EL_TERMINAL, "vt100");
	} else {
	    el_set(RPL_editor, EL_TERMINAL, NULL);
	}
    } else {
	el_set(RPL_editor, EL_TERMINAL, NULL);
    }
#else
    el_set(RPL_editor, EL_TERMINAL, NULL);
#endif

    RPL_interpreter = rpl_interpreter_new(NULL, NULL);
    if (RPL_interpreter == NULL) {
	fprintf(stderr, "RPL: error: Couldn't initialize librpl\n");
	goto error;
    }

    bool done = false;
    while (!done) {

	/* Get input using libedit, for history etc. */

	int line_len = 0;
	const char *line = el_gets(RPL_editor, &line_len);
	if (line == NULL) {
	    if (line_len == -1) {
		/* Error. */
		saved_errno = errno;
		goto error;
	    } else {
		/* No data = done. */
		done = true;
		continue;
	    }
	}

	rpl_unistring_t input = rpl_unistring_new_from_utf8(line,
							    line_len);
	if (input == NULL) {
	    saved_errno = ENOMEM;
	    goto error;
	}

	/* Send the input to the interpreter. */

	bool appended = rpl_interpreter_append_input(RPL_interpreter,
						     input);
	if (appended == false) {
	    saved_errno = ENOMEM;
	    goto error;
	}

	rpl_unistring_release(input);

	/*
	 Step the interpreter and send along any ouptut that it produces
	 until there's nothing more for the interpreter to do.
	 */

	bool ran = false;
	do {
	    ran = rpl_interpreter_step(RPL_interpreter);

	    if (ran) {
		saved_errno = -RPL_send_along_output(RPL_interpreter);
		if (saved_errno != 0) goto error;
	    }
	} while (ran == true);

	/*
	 Send along any output generated even if the interpreter didn't
	 run successfully.
	 */

	saved_errno = -RPL_send_along_output(RPL_interpreter);
	if (saved_errno != 0) goto error;

	/* Output the stack, if there's anything on it. */

	bool produced = rpl_interpreter_output_stack(RPL_interpreter);
	if (produced) {
	    saved_errno = -RPL_send_along_output(RPL_interpreter);
	    if (saved_errno != 0) goto error;
	}
    }

    rpl_interpreter_free(RPL_interpreter);
    history_end(RPL_history);
    el_end(RPL_editor);

    return EX_OK;

error:
    if (saved_errno) {
	fprintf(stderr, "rpl: error: errno = %d\n", saved_errno);
    }
    return EX_SOFTWARE;
}


RPL_SOURCE_END
