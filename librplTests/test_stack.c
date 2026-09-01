//
//  test_stack.c
//  RPL
//
//  Created by Chris Hanson on 8/31/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "test_stack.h"

#include "rpl_real.h"
#include "rpl_stack.h"


RPL_SOURCE_BEGIN


void
test_stack_setup(void)
{
    tests_shared_setup();

    /* Add other setup here. */
}


void
test_stack_teardown(void)
{
    /* Add other teardown here. */

    tests_shared_teardown();
}


/* MARK: - Tests */

START_TEST(test_creation)
{
    rpl_stack_t stack = rpl_stack_new(128);

    ck_assert_ptr_nonnull(stack);
    ck_assert_int_eq(rpl_stack_get_depth(stack), 128);
    ck_assert_int_eq(rpl_stack_get_level(stack), 0);

    rpl_stack_free(stack);
}
END_TEST

START_TEST(test_push_and_pop)
{
    rpl_stack_t stack = rpl_stack_new(128);

    rpl_value_t three_push = rpl_real_new(3);
    rpl_value_t four_push = rpl_real_new(4);

    rpl_stack_push(stack, three_push);
    rpl_value_release(three_push);

    rpl_stack_push(stack, four_push);
    rpl_value_release(four_push);

    ck_assert_int_eq(rpl_stack_get_level(stack), 2);

    rpl_value_t popped_four = rpl_stack_pop(stack);
    ck_assert_ptr_nonnull(popped_four);
    ck_assert_double_eq(rpl_real_get_rep(popped_four), 4);
    rpl_value_release(popped_four);

    ck_assert_int_eq(rpl_stack_get_level(stack), 1);

    rpl_value_t popped_three = rpl_stack_pop(stack);
    ck_assert_ptr_nonnull(popped_three);
    ck_assert_double_eq(rpl_real_get_rep(popped_three), 3);
    rpl_value_release(popped_three);

    ck_assert_int_eq(rpl_stack_get_level(stack), 0);

    rpl_stack_free(stack);
}
END_TEST


/* MARK: - Test Infrastructure */

Suite *
test_stack_suite(void)
{
    Suite *s = suite_create("test_stack");

    TCase *tc_stack = tcase_create("test_stack");
    tcase_add_checked_fixture(tc_stack,
			      test_stack_setup,
			      test_stack_teardown);
    tcase_add_test(tc_stack, test_creation);
    tcase_add_test(tc_stack, test_push_and_pop);

    suite_add_tcase(s, tc_stack);

    return s;
}


RPL_SOURCE_END
