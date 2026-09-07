//
//  test_tokenizer.c
//  librplTests
//
//  Created by Chris Hanson on 9/5/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "test_tokenizer.h"


RPL_SOURCE_BEGIN


rpl_context_t context = NULL;
rpl_tokenizer_t tokenizer = NULL;


void
test_tokenizer_setup(void)
{
    tests_shared_setup();

    context = rpl_context_new();
    ck_assert_ptr_nonnull(context);

    tokenizer = rpl_tokenizer_new(context);
    ck_assert_ptr_nonnull(tokenizer);
}


void
test_tokenizer_teardown(void)
{
    rpl_tokenizer_free(tokenizer);
    rpl_context_free(context);

    tests_shared_teardown();
}


/* MARK: - Tests */

START_TEST(test_binary_integer)
{
    bool appended = rpl_tokenizer_append(tokenizer, "# 17h");
    ck_assert(appended);

    rpl_token_t token = rpl_tokenizer_copy_next(tokenizer);
    ck_assert_ptr_nonnull(token);
    ck_assert_int_eq(rpl_token_type_value, rpl_token_get_type(token));

    rpl_value_t value = rpl_token_get_value(token);
    ck_assert_ptr_nonnull(value);
    ck_assert_int_eq(rpl_type_integer, rpl_value_get_type(value));
    ck_assert_int_eq(23, rpl_integer_get_rep(value));

    rpl_token_free(token);
}
END_TEST

START_TEST(test_name)
{
    bool appended;

    /* Validate basic name tokenization. */
    {
	appended = rpl_tokenizer_append(tokenizer, "'NAME'");
	ck_assert(appended);

	rpl_token_t token = rpl_tokenizer_copy_next(tokenizer);
	ck_assert_ptr_nonnull(token);
	ck_assert_int_eq(rpl_token_type_value,
			 rpl_token_get_type(token));

	rpl_value_t value = rpl_token_get_value(token);
	ck_assert_ptr_nonnull(value);
	ck_assert_int_eq(rpl_type_name, rpl_value_get_type(value));
	ck_assert_int_eq(4, rpl_name_get_rep_len(value));
	ck_assert_str_eq("NAME", rpl_name_get_rep(value));

	rpl_token_free(token);
    }

    /* Validate tokenization of a name used to represent units. */
    {
	appended = rpl_tokenizer_append(tokenizer, "'m/s^2'");
	ck_assert(appended);

	rpl_token_t token = rpl_tokenizer_copy_next(tokenizer);
	ck_assert_ptr_nonnull(token);
	ck_assert_int_eq(rpl_token_type_value,
			 rpl_token_get_type(token));

	rpl_value_t value = rpl_token_get_value(token);
	ck_assert_ptr_nonnull(value);
	ck_assert_int_eq(rpl_type_name, rpl_value_get_type(value));
	ck_assert_int_eq(5, rpl_name_get_rep_len(value));
	ck_assert_str_eq("m/s^2", rpl_name_get_rep(value));

	rpl_token_free(token);
    }
}
END_TEST

START_TEST(test_string)
{
    bool appended = rpl_tokenizer_append(tokenizer,
					 "\"Hello, world!\\r\\n\"");
    ck_assert(appended);

    rpl_token_t token = rpl_tokenizer_copy_next(tokenizer);
    ck_assert_ptr_nonnull(token);
    ck_assert_int_eq(rpl_token_type_value, rpl_token_get_type(token));

    rpl_value_t value = rpl_token_get_value(token);
    ck_assert_ptr_nonnull(value);
    ck_assert_int_eq(rpl_type_string, rpl_value_get_type(value));
    ck_assert_int_eq(15, rpl_string_get_rep_len(value)); /* no quotes */
    ck_assert_str_eq("Hello, world!\r\n", rpl_string_get_rep(value));

    rpl_token_free(token);
}
END_TEST


/* MARK: - Test Infrastructure */

Suite *
test_tokenizer_suite(void)
{
    Suite *s = suite_create("test_tokenizer");

    TCase *tc_tokenizer = tcase_create("test_tokenizer");
    tcase_add_checked_fixture(tc_tokenizer,
			      test_tokenizer_setup,
			      test_tokenizer_teardown);
    tcase_add_test(tc_tokenizer, test_binary_integer);
    tcase_add_test(tc_tokenizer, test_name);
    tcase_add_test(tc_tokenizer, test_string);

    suite_add_tcase(s, tc_tokenizer);

    return s;
}


RPL_SOURCE_END
