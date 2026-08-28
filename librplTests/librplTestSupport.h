//
//  librplTestSupport.h
//  librplTests
//
//  Created by Chris Hanson on 8/28/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__librplTestSupport__h__
#define __RPL__librplTestSupport__h__

#include <check.h>

#include <stdbool.h>

#include "rpl.h"


RPL_HEADER_BEGIN

/*!
 Get the complete, configured test suite runner.

 Get the test suite covering all tests and a test runner that can be
 used to run it, without actually running any of the tests. In the
 future (with some API enhancements to Check) this will allow the test
 suite to be run under programmatic control for hooking into some other
 test infrastructure (such as Xcode's).
 */
RPL_EXPORT
SRunner * RPL_NULLABLE
configured_tests_runner(void);


/*!
 Main entry point of tests.

 Run the full suite of tests as if from the command line.
 */
RPL_EXPORT
int
tests_main(int argc,
	   char * RPL_NULLABLE * RPL_NONNULL argv);


/*! Setup shared by all tests. */
RPL_EXPORT
void
tests_shared_setup(void);


/*! Teardown shared by all tests. */
RPL_EXPORT
void
tests_shared_teardown(void);


/*!
 Read a test file.

 Reads a test file from the `test` directory, returning its
 contents and size on success.

 - WARNING: The result buffer is allocated with `calloc(3)` and is the
	    caller's responsibility to dealloate with `free(3)`.
 */
RPL_EXPORT
bool
tests_read_file(const char *name,
		char * RPL_NULLABLE * RPL_NONNULL data,
		size_t * RPL_NONNULL data_len);


RPL_HEADER_END


#endif /* __RPL__librplTestSupport__h__ */
