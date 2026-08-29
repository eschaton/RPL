//
//  test_integer.c
//  librplTests
//
//  Created by Chris Hanson on 8/28/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "test_integer.h"

#include "rpl_integer_internal.h"


RPL_SOURCE_BEGIN


void
test_integer_setup(void)
{
    tests_shared_setup();

    /* Add other setup here. */
}


void
test_integer_teardown(void)
{
    /* Add other teardown here. */

    tests_shared_teardown();
}


/* MARK: - Tests */

START_TEST(test_creation)
{
    rpl_value_t v0 = rpl_integer_new(0);
    ck_assert_ptr_nonnull(v0);
    ck_assert_int_eq(rpl_integer_get_rep(v0), 0);
    ck_assert_int_eq(rpl_value_get_type(v0), rpl_type_integer);

    rpl_value_release(v0);
}
END_TEST

START_TEST(test_round_to_next)
{
    rpl_integer_t quantum = 16;
    rpl_integer_t base = 5;
    rpl_integer_t next = rpl_integer_round_to_next(base, quantum);
    ck_assert_int_eq(next, quantum);
}
END_TEST


/* MARK: - Test Infrastructure */

Suite *
test_integer_suite(void)
{
    Suite *s = suite_create("test_integer");

    TCase *tc_integer = tcase_create("test_integer");
    tcase_add_checked_fixture(tc_integer,
			      test_integer_setup,
			      test_integer_teardown);
    tcase_add_test(tc_integer, test_creation);
    tcase_add_test(tc_integer, test_round_to_next);

    suite_add_tcase(s, tc_integer);

    return s;
}


RPL_SOURCE_END
