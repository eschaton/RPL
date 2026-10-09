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
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>

#include "rpl.h"


RPL_SOURCE_BEGIN


EditLine * RPL_NULLABLE RPL_editor = NULL;
History * RPL_NULLABLE RPL_history = NULL;
rpl_interpreter_t RPL_interpreter = NULL;


/*! Produce a prompt for the command line. */
char * RPL_NONNULL
RPL_prompt(EditLine *editor)
{
    return "RPL> ";
}

/*!
 Configure `libedit` for RPL use.

 - Returns: 0 on success, -`errno` on failure
 */
int
RPL_configure_libedit(void)
{
    int saved_errno = 0;

    RPL_editor = el_init("RPL", stdin, stdout, stderr);
    if (RPL_editor == NULL) {
	saved_errno = errno;
	goto error;
    }

    RPL_history = history_init();
    if (RPL_history == NULL) {
	saved_errno = errno;
	goto error;
    }

    /* Pull in the .editrc */

    int parsed = el_source(RPL_editor, NULL);
    if (parsed != 0) {
	saved_errno = errno;
	if (saved_errno == ENOENT) {
	    /* No .editrc is success. */
	} else {
	    fprintf(stderr, "RPL: error: Couldn't parse .editrc\n");
	    goto error;
	}
    }


    /*
     Determine whether editing is enabled, if it's not then don't do a
     bunch of registration and stop using libedit, which will set the
     `RPL_editor` global to `NULL` as a signal.
     */

    int editmode = 0;
    el_get(RPL_editor, EL_EDITMODE, &editmode);

    if (editmode) {
	/* Use/keep infinite command line history. */

	HistEvent histev;
	history(RPL_history, &histev, H_SETSIZE, INT_MAX);
	el_set(RPL_editor, EL_HIST, history, RPL_history);

	/*
	 Prefer emacs-style command line editing, set prompt, and enable
	 default libedit signal handling.
	 */

	el_set(RPL_editor, EL_EDITOR, "emacs");
	el_set(RPL_editor, EL_PROMPT, RPL_prompt);
	el_set(RPL_editor, EL_SIGNAL, 1);

#if __APPLE__
	/*
	 When running under Xcode's debugger, "TERM=dumb" can be set
	 even when using Terminal.app rather than the debugger UI for
	 I/O with RPL. In that case, specify the terminal is actually
	 "xterm-256color", since Terminal.app supports that; otherwise,
	 set the terminal from the environment. Filed as FB25118078.
	 */
	const char *TERM = getenv("TERM");
	if ((getenv("__XCODE_BUILT_PRODUCTS_DIR_PATHS") != NULL)
	    && TERM && (strcmp(TERM, "dumb") == 0))
	{
	    el_set(RPL_editor, EL_TERMINAL, "xterm-256color");
	} else {
	    el_set(RPL_editor, EL_TERMINAL, NULL);
	}
#else
	/* Set the terminal type to use from the environment. */

	el_set(RPL_editor, EL_TERMINAL, NULL);
#endif
    } else {
	el_end(RPL_editor); RPL_editor = NULL;
	history_end(RPL_history); RPL_history = NULL;
    }

    return 0;

error:
    if (RPL_editor) el_end(RPL_editor);
    if (RPL_history) history_end(RPL_history);
    return -saved_errno;
}

/*!
 Get a line of input from `stdin` and return it as a Unicode string.

 - Returns: a Unicode string containing a line of input, an empty string
            on end of file, or `NULL` (with `errno` set) on error
 */
rpl_unistring_t RPL_NULLABLE
RPL_input_copy(void)
RPL_RETURNS_RETAINED
{
    rpl_unistring_t line_str = NULL;
    bool saw_eof;
    int saved_errno;
    char buf[1024];
    const char *line = NULL;
    int line_len = 0;

    line_str = rpl_unistring_new(0);
    if (line_str == NULL) {
	saved_errno = ENOMEM;
	goto error;
    }

    if (RPL_editor) {
	line = el_gets(RPL_editor, &line_len);
	if (line == NULL) {
	    if (line_len < 0) {
		saved_errno = errno;
		saw_eof = false;
	    } else {
		saved_errno = 0;
		saw_eof = true;
	    }
	} else {
	    saved_errno = 0;
	    saw_eof = false;
	}

	if (saved_errno != 0) goto error;
    } else {
	line = fgets(buf, 1024, stdin);
	if (line == NULL) {
	    saved_errno = errno;
	    if (feof(stdin)) {
		line = "";
		saw_eof = true;
	    } else {
		goto error;
	    }
	} else {
	    saw_eof = false;
	}
    }

    if (saw_eof == false) {
	rpl_unistring_t another = rpl_unistring_new_from_utf8(line,
							      line_len);
	if (another) {
	    bool appended = rpl_unistring_append(line_str, another);
	    rpl_unistring_release(another);
	    if (appended == false) {
		saved_errno = ENOMEM;
		goto error;
	    }
	} else {
	    saved_errno = ENOMEM;
	    goto error;
	}
    } else {
	/*
	 Leave line empty, that's the signal to the caller that EOF was
	 encountered.
	 */
    }

    return line_str;

error:
    if (line_str) rpl_unistring_release(line_str);
    errno = saved_errno;
    return NULL;
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

    saved_errno = -RPL_configure_libedit();
    if (saved_errno != 0) {
	fprintf(stderr, "RPL: error: Couldn't configure libedit\n");
	goto error;
    }

    RPL_interpreter = rpl_interpreter_new(NULL, NULL);
    if (RPL_interpreter == NULL) {
	fprintf(stderr, "RPL: error: Couldn't initialize librpl\n");
	goto error;
    }

    /* Main loop! */

    bool done = false;
    while (!done) {
	/* Get input from stdin. */

	rpl_unistring_t input = RPL_input_copy();
	if (input == NULL) {
	    saved_errno = ENOMEM;
	    goto error;
	} else {
	    /* EOF is signaled by an empty string. */

	    if (rpl_unistring_get_length(input) == 0) {
		rpl_unistring_release(input);
		done = true;
		continue;
	    }
	}

	/* Send the input to the interpreter. */

	bool appended = rpl_interpreter_append_input(RPL_interpreter,
						     input);
	rpl_unistring_release(input);
	if (appended == false) {
	    saved_errno = ENOMEM;
	    goto error;
	}

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
    if (RPL_editor) el_end(RPL_editor);
    if (RPL_history) history_end(RPL_history);

    return EX_OK;

error:
    if (saved_errno) {
	fprintf(stderr, "rpl: error: errno = %d\n", saved_errno);
    }
    return EX_SOFTWARE;
}


RPL_SOURCE_END
