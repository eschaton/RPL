//
//  test_real.c
//  librplTests
//
//  Created by Chris Hanson on 9/3/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "test_real.h"

#include "rpl_real_internal.h"

#include <stdlib.h>


RPL_SOURCE_BEGIN


void
test_real_setup(void)
{
    tests_shared_setup();

    /* Add other setup here. */
}


void
test_real_teardown(void)
{
    /* Add other teardown here. */

    tests_shared_teardown();
}


/* MARK: - Tests */

START_TEST(test_creation)
{
    rpl_value_t v0 = rpl_real_new(0);
    ck_assert_ptr_nonnull(v0);
    ck_assert_double_eq(rpl_real_get_rep(v0), 0);
    ck_assert_int_eq(rpl_value_get_type(v0), rpl_type_real);

    rpl_value_release(v0);
}
END_TEST

START_TEST(test_printing)
{
    rpl_value_t three = rpl_real_new(3);
    ck_assert_ptr_nonnull(three);

    rpl_value_t four = rpl_real_new(4);
    ck_assert_ptr_nonnull(four);

    rpl_unistring_t three_str = rpl_value_copy_string(three, NULL);
    ck_assert_ptr_nonnull(three_str);
    char *three_utf8 = rpl_unistring_copy_utf8(three_str);
    ck_assert_ptr_nonnull(three_utf8);
    ck_assert_str_eq(three_utf8, "3");
    rpl_unistring_release(three_str);
    free(three_utf8);

    rpl_unistring_t four_str = rpl_value_copy_string(four, NULL);
    ck_assert_ptr_nonnull(four_str);
    char *four_utf8 = rpl_unistring_copy_utf8(four_str);
    ck_assert_ptr_nonnull(four_utf8);
    ck_assert_str_eq(four_utf8, "4");
    rpl_unistring_release(four_str);
    free(four_utf8);

    rpl_value_release(three);
    rpl_value_retain(four);
}
END_TEST

START_TEST(test_printing_angle)
{
    rpl_environment_t env = rpl_environment_new();
    ck_assert_ptr_nonnull(env);

    /* The default angle mode should be degrees. */
    ck_assert_int_eq(rpl_environment_get_angle_mode(env),
		     rpl_angle_mode_degrees);

    rpl_value_t qr = rpl_real_pi_div_2();
    ck_assert_ptr_nonnull(qr);

    rpl_unistring_t qr_str = rpl_real_copy_angle_string(qr, env);
    ck_assert_ptr_nonnull(qr_str);
    char *qr_utf8 = rpl_unistring_copy_utf8(qr_str);
    ck_assert_ptr_nonnull(qr_utf8);
    ck_assert_str_eq(qr_utf8, "∡90"); /* π/2 radians = 90° */
    rpl_unistring_release(qr_str);
    free(qr_utf8);

    rpl_environment_set_angle_mode(env, rpl_angle_mode_radians);
    rpl_environment_set_coordinate_system(env,
	rpl_coordinate_system_spherical);

    rpl_unistring_t qr2_str = rpl_real_copy_angle_string(qr, env);
    ck_assert_ptr_nonnull(qr2_str);
    char *qr2_utf8 = rpl_unistring_copy_utf8(qr2_str);
    ck_assert_str_eq(qr2_utf8, "∢1.5708"); /* π/2 radians = 1.570796 */
    rpl_unistring_release(qr2_str);
    free(qr2_utf8);

    rpl_value_release(qr);

    rpl_environment_free(env);
}
END_TEST


/* MARK: - Test Infrastructure */

Suite *
test_real_suite(void)
{
    Suite *s = suite_create("test_real");

    TCase *tc_real = tcase_create("test_real");
    tcase_add_checked_fixture(tc_real,
			      test_real_setup,
			      test_real_teardown);
    tcase_add_test(tc_real, test_creation);
    tcase_add_test(tc_real, test_printing);
    tcase_add_test(tc_real, test_printing_angle);

    suite_add_tcase(s, tc_real);

    return s;
}


RPL_SOURCE_END
