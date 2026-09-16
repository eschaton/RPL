//
//  test_unistring.c
//  librplTests
//
//  Created by Chris Hanson on 9/8/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "test_unistring.h"

#include <stdlib.h>

#include "rpl_unistring_internal.h"


RPL_SOURCE_BEGIN


void
test_unistring_setup(void)
{
    tests_shared_setup();

    /* Add other setup here. */
}


void
test_unistring_teardown(void)
{
    /* Add other teardown here. */

    tests_shared_teardown();
}


/* MARK: - Tests */

START_TEST(test_creation)
{
    rpl_unistring_t str = rpl_unistring_new(0);
    ck_assert_ptr_nonnull(str);
    ck_assert_int_eq(0, str->_count);
    ck_assert_int_eq(16, str->_capacity);
    ck_assert_int_eq(1, str->_refcnt);

    rpl_unistring_release(str);
}
END_TEST

START_TEST(test_conversion_from_utf8)
{
    rpl_unistring_t hello_world
	= rpl_unistring_new_from_utf8("Hello, world!", 13);
    ck_assert_ptr_nonnull(hello_world);
    ck_assert_int_eq(13, hello_world->_count);

    ck_assert_int_eq((rpl_unichar_t) 'H', hello_world->_storage[0]);
    ck_assert_int_eq((rpl_unichar_t) 'e', hello_world->_storage[1]);
    ck_assert_int_eq((rpl_unichar_t) 'l', hello_world->_storage[2]);
    ck_assert_int_eq((rpl_unichar_t) 'l', hello_world->_storage[3]);
    ck_assert_int_eq((rpl_unichar_t) 'o', hello_world->_storage[4]);
    ck_assert_int_eq((rpl_unichar_t) ',', hello_world->_storage[5]);
    ck_assert_int_eq((rpl_unichar_t) ' ', hello_world->_storage[6]);
    ck_assert_int_eq((rpl_unichar_t) 'w', hello_world->_storage[7]);
    ck_assert_int_eq((rpl_unichar_t) 'o', hello_world->_storage[8]);
    ck_assert_int_eq((rpl_unichar_t) 'r', hello_world->_storage[9]);
    ck_assert_int_eq((rpl_unichar_t) 'l', hello_world->_storage[10]);
    ck_assert_int_eq((rpl_unichar_t) 'd', hello_world->_storage[11]);
    ck_assert_int_eq((rpl_unichar_t) '!', hello_world->_storage[12]);

    rpl_unistring_release(hello_world);

    rpl_unistring_t program
	= rpl_unistring_new_from_utf8("« program »", 13);
    ck_assert_ptr_nonnull(program);
    ck_assert_int_eq(11, program->_count);

    ck_assert_int_eq(0x000000AB, program->_storage[0]);
    ck_assert_int_eq(0x000000BB, program->_storage[10]);

    rpl_unistring_release(program);
}
END_TEST

START_TEST(test_conversion_to_utf8)
{
    {
	rpl_unichar_t hello_world_arr[] = {
	    'H', 'e', 'l', 'l', 'o', ',', ' ',
	    'w', 'o', 'r', 'l', 'd', '!'
	};

	rpl_unistring_t hello_world
	= rpl_unistring_new_from_chars(hello_world_arr, 13);
	ck_assert_ptr_nonnull(hello_world);
	ck_assert_int_eq(13, hello_world->_count);

	char *hello_world_utf8 = rpl_unistring_copy_utf8(hello_world);
	ck_assert_ptr_nonnull(hello_world_utf8);
	ck_assert_str_eq("Hello, world!", hello_world_utf8);
	ck_assert_int_eq(13, strlen(hello_world_utf8));

	rpl_unistring_release(hello_world);
	free(hello_world_utf8);
    }

    {
	rpl_unichar_t program_arr[] = {
	    rpl_unichar_chevron_open, ' ',
	    'p', 'r', 'o', 'g', 'r', 'a', 'm',
	    ' ', rpl_unichar_chevron_close,
	};

	rpl_unistring_t program
	    = rpl_unistring_new_from_chars(program_arr, 11);
	ck_assert_ptr_nonnull(program);
	ck_assert_int_eq(11, program->_count);

	char *program_utf8 = rpl_unistring_copy_utf8(program);
	ck_assert_ptr_nonnull(program_utf8);
	ck_assert_str_eq("« program »", program_utf8);
	ck_assert_int_eq(13, strlen(program_utf8));

	rpl_unistring_release(program);
	free(program_utf8);
    }
}
END_TEST

START_TEST(test_remove_all)
{
    rpl_unistring_t hello_world
	= rpl_unistring_new_from_utf8("Hello, world!", 13);
    ck_assert_ptr_nonnull(hello_world);
    ck_assert_int_eq(13, hello_world->_count);

    rpl_unistring_remove_all(hello_world);
    ck_assert_int_eq(0, hello_world->_count);

    rpl_unistring_release(hello_world);
}
END_TEST


/* MARK: - Test Infrastructure */

Suite *
test_unistring_suite(void)
{
    Suite *s = suite_create("test_unistring");

    TCase *tc_unistring = tcase_create("test_unistring");
    tcase_add_checked_fixture(tc_unistring,
			      test_unistring_setup,
			      test_unistring_teardown);
    tcase_add_test(tc_unistring, test_creation);
    tcase_add_test(tc_unistring, test_conversion_from_utf8);
    tcase_add_test(tc_unistring, test_conversion_to_utf8);
    tcase_add_test(tc_unistring, test_remove_all);

    suite_add_tcase(s, tc_unistring);

    return s;
}


RPL_SOURCE_END
