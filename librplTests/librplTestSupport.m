//
//  librplTestSupport.c
//  librplTests
//
//  Created by Chris Hanson on 8/21/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "librplTestSupport.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_adjbuffer.h"
#include "test_integer.h"


RPL_SOURCE_BEGIN


SRunner * RPL_NULLABLE
configured_tests_runner(void)
{
    SRunner *sr = srunner_create(NULL);

    srunner_add_suite(sr, test_adjbuffer_suite());
    srunner_add_suite(sr, test_integer_suite());

    return sr;
}


int
tests_main(int argc,
	   char * RPL_NULLABLE * RPL_NONNULL argv)
{
    SRunner *sr = configured_tests_runner();

    srunner_run_all(sr, CK_VERBOSE);

    int number_failed = srunner_ntests_failed(sr);

    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


void tests_shared_setup(void)
{
    // Do nothing, for now.
}


void tests_shared_teardown(void)
{
    // Do nothing, for now.
}


bool
tests_read_file(const char *name,
		char * RPL_NULLABLE * RPL_NONNULL data,
		size_t * RPL_NONNULL data_len)
{
    char fullpath[PATH_MAX] = {0};
    snprintf(fullpath, PATH_MAX, "tests/%s", name);

    FILE *f = fopen(fullpath, "r");
    if (f) {
	int seek_err = fseeko(f, 0, SEEK_END);
	if (seek_err != -1) {
	    off_t f_len = ftello(f);
	    if (f_len != -1) {
		int reset_err = fseeko(f, 0, SEEK_SET);
		if (reset_err != -1) {
		    char *buf = calloc(f_len, sizeof(char));
		    if (buf != NULL) {
			ssize_t items = fread(buf, f_len, 1, f);
			if (items == 1) {
			    *data = buf;
			    *data_len = f_len;
			    return true;
			} else {
			    free(buf);
			    buf = NULL;
			}
		    }
		}
	    }
	}

	fclose(f);
    }

    return false;
}


RPL_SOURCE_END
