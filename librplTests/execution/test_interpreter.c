//
//  test_interpreter.c
//  RPL
//
//  Created by Chris Hanson on 9/12/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "test_interpreter.h"

#include "rpl_interpreter.h"


RPL_SOURCE_BEGIN


void
test_interpreter_setup(void)
{
    tests_shared_setup();

    /* Add other setup here. */
}


void
test_interpreter_teardown(void)
{
    /* Add other teardown here. */

    tests_shared_teardown();
}


/* MARK: - Tests */

START_TEST(test_creation)
{
    rpl_interpreter_t interpreter = rpl_interpreter_new(NULL, NULL);
    ck_assert_ptr_nonnull(interpreter);

    rpl_interpreter_free(interpreter);
}
END_TEST


/* MARK: - Test Infrastructure */

Suite *
test_interpreter_suite(void)
{
    Suite *s = suite_create("test_interpreter");

    TCase *tc_interpreter = tcase_create("test_interpreter");
    tcase_add_checked_fixture(tc_interpreter,
			      test_interpreter_setup,
			      test_interpreter_teardown);
    tcase_add_test(tc_interpreter, test_creation);

    suite_add_tcase(s, tc_interpreter);

    return s;
}


RPL_SOURCE_END
