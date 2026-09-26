//
//  test_tokenizer.c
//  librplTests
//
//  Created by Chris Hanson on 9/5/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "test_tokenizer.h"

#include <stdlib.h>

#include "rpl_identifier.h"


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
    const char *text = "# 17h";
    const size_t text_len = strlen(text);
    rpl_unistring_t buf = rpl_unistring_new_from_utf8(text, text_len);
    ck_assert_ptr_nonnull(buf);
    bool appended = rpl_tokenizer_append(tokenizer, buf);
    ck_assert(appended);
    rpl_unistring_release(buf);

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

START_TEST(test_real)
{
    const char *text = "1 -1 .1 -.1 0.1 1e2 1E-3\n";
    const size_t text_len = strlen(text);
    rpl_unistring_t buf = rpl_unistring_new_from_utf8(text,
						      text_len);
    ck_assert_ptr_nonnull(buf);
    bool appended = rpl_tokenizer_append(tokenizer, buf);
    ck_assert(appended);
    rpl_unistring_release(buf);

    {
	rpl_token_t token = rpl_tokenizer_copy_next(tokenizer);
	ck_assert_ptr_nonnull(token);
	ck_assert_int_eq(rpl_token_type_value,
			 rpl_token_get_type(token));

	rpl_value_t value = rpl_token_get_value(token);
	ck_assert_ptr_nonnull(value);
	ck_assert_int_eq(rpl_type_real, rpl_value_get_type(value));
	ck_assert_double_eq(1.0, rpl_real_get_rep(value));

	rpl_token_free(token);
    }

    {
	rpl_token_t token = rpl_tokenizer_copy_next(tokenizer);
	ck_assert_ptr_nonnull(token);
	ck_assert_int_eq(rpl_token_type_value,
			 rpl_token_get_type(token));

	rpl_value_t value = rpl_token_get_value(token);
	ck_assert_ptr_nonnull(value);
	ck_assert_int_eq(rpl_type_real, rpl_value_get_type(value));
	ck_assert_double_eq(-1.0, rpl_real_get_rep(value));

	rpl_token_free(token);
    }

    {
	rpl_token_t token = rpl_tokenizer_copy_next(tokenizer);
	ck_assert_ptr_nonnull(token);
	ck_assert_int_eq(rpl_token_type_value,
			 rpl_token_get_type(token));

	rpl_value_t value = rpl_token_get_value(token);
	ck_assert_ptr_nonnull(value);
	ck_assert_int_eq(rpl_type_real, rpl_value_get_type(value));
	ck_assert_double_eq(0.1, rpl_real_get_rep(value));

	rpl_token_free(token);
    }

    {
	rpl_token_t token = rpl_tokenizer_copy_next(tokenizer);
	ck_assert_ptr_nonnull(token);
	ck_assert_int_eq(rpl_token_type_value,
			 rpl_token_get_type(token));

	rpl_value_t value = rpl_token_get_value(token);
	ck_assert_ptr_nonnull(value);
	ck_assert_int_eq(rpl_type_real, rpl_value_get_type(value));
	ck_assert_double_eq(-0.1, rpl_real_get_rep(value));

	rpl_token_free(token);
    }

    {
	rpl_token_t token = rpl_tokenizer_copy_next(tokenizer);
	ck_assert_ptr_nonnull(token);
	ck_assert_int_eq(rpl_token_type_value,
			 rpl_token_get_type(token));

	rpl_value_t value = rpl_token_get_value(token);
	ck_assert_ptr_nonnull(value);
	ck_assert_int_eq(rpl_type_real, rpl_value_get_type(value));
	ck_assert_double_eq(0.1, rpl_real_get_rep(value));

	rpl_token_free(token);
    }

    {
	rpl_token_t token = rpl_tokenizer_copy_next(tokenizer);
	ck_assert_ptr_nonnull(token);
	ck_assert_int_eq(rpl_token_type_value,
			 rpl_token_get_type(token));

	rpl_value_t value = rpl_token_get_value(token);
	ck_assert_ptr_nonnull(value);
	ck_assert_int_eq(rpl_type_real, rpl_value_get_type(value));
	ck_assert_double_eq(100, rpl_real_get_rep(value));

	rpl_token_free(token);
    }

    {
	rpl_token_t token = rpl_tokenizer_copy_next(tokenizer);
	ck_assert_ptr_nonnull(token);
	ck_assert_int_eq(rpl_token_type_value,
			 rpl_token_get_type(token));

	rpl_value_t value = rpl_token_get_value(token);
	ck_assert_ptr_nonnull(value);
	ck_assert_int_eq(rpl_type_real, rpl_value_get_type(value));
	ck_assert_double_eq(0.001, rpl_real_get_rep(value));

	rpl_token_free(token);
    }
}
END_TEST

START_TEST(test_name)
{
    bool appended;

    /* Validate basic name tokenization. */
    {
	const char *text = "'NAME'";
	const size_t text_len = strlen(text);
	rpl_unistring_t buf = rpl_unistring_new_from_utf8(text, text_len);
	ck_assert_ptr_nonnull(buf);
	appended = rpl_tokenizer_append(tokenizer, buf);
	ck_assert(appended);
	rpl_unistring_release(buf);

	rpl_token_t token = rpl_tokenizer_copy_next(tokenizer);
	ck_assert_ptr_nonnull(token);
	ck_assert_int_eq(rpl_token_type_value,
			 rpl_token_get_type(token));

	rpl_value_t value = rpl_token_get_value(token);
	ck_assert_ptr_nonnull(value);
	ck_assert_int_eq(rpl_type_name, rpl_value_get_type(value));
	rpl_unistring_t rep = rpl_name_get_rep(value);
	ck_assert_ptr_nonnull(rep);
	char *rep_utf8 = rpl_unistring_copy_utf8(rep);
	ck_assert_ptr_nonnull(rep_utf8);
	ck_assert_str_eq("NAME", rep_utf8);
	free(rep_utf8);

	rpl_token_free(token);
    }

    /* Validate tokenization of a name used to represent units. */
    {
	const char *text = "'m/s^2'";
	const size_t text_len = strlen(text);
	rpl_unistring_t buf = rpl_unistring_new_from_utf8(text, text_len);
	ck_assert_ptr_nonnull(buf);
	appended = rpl_tokenizer_append(tokenizer, buf);
	ck_assert(appended);
	rpl_unistring_release(buf);

	rpl_token_t token = rpl_tokenizer_copy_next(tokenizer);
	ck_assert_ptr_nonnull(token);
	ck_assert_int_eq(rpl_token_type_value,
			 rpl_token_get_type(token));

	rpl_value_t value = rpl_token_get_value(token);
	ck_assert_ptr_nonnull(value);
	ck_assert_int_eq(rpl_type_name, rpl_value_get_type(value));
	rpl_unistring_t rep = rpl_name_get_rep(value);
	ck_assert_ptr_nonnull(rep);
	char *rep_utf8 = rpl_unistring_copy_utf8(rep);
	ck_assert_ptr_nonnull(rep_utf8);
	ck_assert_str_eq("m/s^2", rep_utf8);
	free(rep_utf8);

	rpl_token_free(token);
    }
}
END_TEST

START_TEST(test_string)
{
    const char *text = "\"Hello, world!\\r\\n\"";
    const size_t text_len = strlen(text);
    rpl_unistring_t buf = rpl_unistring_new_from_utf8(text, text_len);
    ck_assert_ptr_nonnull(buf);
    bool appended = rpl_tokenizer_append(tokenizer, buf);
    ck_assert(appended);
    rpl_unistring_release(buf);

    rpl_token_t token = rpl_tokenizer_copy_next(tokenizer);
    ck_assert_ptr_nonnull(token);
    ck_assert_int_eq(rpl_token_type_value, rpl_token_get_type(token));

    rpl_value_t value = rpl_token_get_value(token);
    ck_assert_ptr_nonnull(value);
    ck_assert_int_eq(rpl_type_string, rpl_value_get_type(value));
    rpl_unistring_t rep = rpl_string_get_rep(value);
    ck_assert_ptr_nonnull(rep);
    char *rep_utf8 = rpl_unistring_copy_utf8(rep);
    ck_assert_ptr_nonnull(rep_utf8);
    ck_assert_int_eq(15, rpl_unistring_get_length(rep)); /* no quotes */
    ck_assert_str_eq(rep_utf8, "Hello, world!\r\n");
    free(rep_utf8);

    rpl_token_free(token);
}
END_TEST

START_TEST(test_identifier)
{
    const char *text = "hello world\n";
    const size_t text_len = strlen(text);
    rpl_unistring_t buf = rpl_unistring_new_from_utf8(text, text_len);
    ck_assert_ptr_nonnull(buf);
    bool appended = rpl_tokenizer_append(tokenizer, buf);
    ck_assert(appended);
    rpl_unistring_release(buf);

    {
	rpl_token_t token = rpl_tokenizer_copy_next(tokenizer);
	ck_assert_ptr_nonnull(token);
	ck_assert_int_eq(rpl_token_type_identifier,
			 rpl_token_get_type(token));

	rpl_unistring_t string = rpl_token_get_string(token);
	ck_assert_ptr_nonnull(string);
	char *string_utf8 = rpl_unistring_copy_utf8(string);
	ck_assert_ptr_nonnull(string_utf8);
	ck_assert_int_eq(5, rpl_unistring_get_length(string));
	ck_assert_str_eq(string_utf8, "hello");
	free(string_utf8);

	rpl_token_free(token);
    }

    {
	rpl_token_t token = rpl_tokenizer_copy_next(tokenizer);
	ck_assert_ptr_nonnull(token);
	ck_assert_int_eq(rpl_token_type_identifier,
			 rpl_token_get_type(token));

	rpl_unistring_t string = rpl_token_get_string(token);
	ck_assert_ptr_nonnull(string);
	char *string_utf8 = rpl_unistring_copy_utf8(string);
	ck_assert_ptr_nonnull(string_utf8);
	ck_assert_int_eq(5, rpl_unistring_get_length(string));
	ck_assert_str_eq(string_utf8, "world");
	free(string_utf8);

	rpl_token_free(token);
    }
}
END_TEST

START_TEST(test_program)
{
    const char *text = "« DUP + »\n";
    const size_t text_len = strlen(text);
    rpl_unistring_t buf = rpl_unistring_new_from_utf8(text, text_len);
    ck_assert_ptr_nonnull(buf);
    bool appended = rpl_tokenizer_append(tokenizer, buf);
    ck_assert(appended);
    rpl_unistring_release(buf);

    rpl_token_t token = rpl_tokenizer_copy_next(tokenizer);
    ck_assert_ptr_nonnull(token);
    ck_assert_int_eq(rpl_token_type_value,
		     rpl_token_get_type(token));

    rpl_value_t value = rpl_token_get_value(token);
    ck_assert_ptr_nonnull(value);
    ck_assert_int_eq(rpl_type_program, rpl_value_get_type(value));

    ck_assert_int_eq(2, rpl_program_get_count(value));

    rpl_value_t dup = rpl_program_get_value(value, 0);
    ck_assert_ptr_nonnull(dup);
    ck_assert_int_eq(rpl_type_identifier, rpl_value_get_type(dup));
    rpl_unistring_t dup_str = rpl_identifier_get_rep(dup);
    ck_assert_ptr_nonnull(dup_str);
    const char *dup_utf8 = rpl_unistring_copy_utf8(dup_str);
    ck_assert_ptr_nonnull(dup_str);
    ck_assert_str_eq("DUP", dup_utf8);
    free((void *)dup_utf8);

    rpl_value_t plus = rpl_program_get_value(value, 1);
    ck_assert_ptr_nonnull(plus);
    ck_assert_int_eq(rpl_type_identifier, rpl_value_get_type(plus));
    rpl_unistring_t plus_str = rpl_identifier_get_rep(plus);
    ck_assert_ptr_nonnull(plus_str);
    const char *plus_utf8 = rpl_unistring_copy_utf8(plus_str);
    ck_assert_ptr_nonnull(plus_str);
    ck_assert_str_eq("+", plus_utf8);
    free((void *)plus_utf8);

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
    tcase_add_test(tc_tokenizer, test_real);
    tcase_add_test(tc_tokenizer, test_name);
    tcase_add_test(tc_tokenizer, test_string);
    tcase_add_test(tc_tokenizer, test_identifier);
    tcase_add_test(tc_tokenizer, test_program);

    suite_add_tcase(s, tc_tokenizer);

    return s;
}


RPL_SOURCE_END
