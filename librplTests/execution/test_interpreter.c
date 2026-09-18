//
//  test_interpreter.c
//  librplTests
//
//  Created by Chris Hanson on 9/12/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "test_interpreter.h"

#include "rpl_interpreter.h"


RPL_SOURCE_BEGIN


rpl_interpreter_t interpreter = NULL;

void
test_interpreter_setup(void)
{
    tests_shared_setup();

    interpreter = rpl_interpreter_new(NULL, NULL);
}


void
test_interpreter_teardown(void)
{
    rpl_interpreter_free(interpreter);

    tests_shared_teardown();
}


/* MARK: - Tests */

START_TEST(test_creation)
{
    ck_assert_ptr_nonnull(interpreter);
}
END_TEST

START_TEST(test_trivial_execution)
{
    const char * text_c = "123\n"
			  "DUP\n";
    const size_t text_c_len = strlen(text_c);
    rpl_unistring_t text = rpl_unistring_new_from_utf8(text_c,
						       text_c_len);
    ck_assert_ptr_nonnull(text);

    bool appended = rpl_interpreter_append_input(interpreter, text);
    ck_assert(appended);

    bool did_push = rpl_interpreter_step(interpreter);
    ck_assert(did_push);

    bool did_DUP = rpl_interpreter_step(interpreter);
    ck_assert(did_DUP);

    rpl_context_t context = rpl_interpreter_get_context(interpreter);
    ck_assert_ptr_nonnull(context);

    rpl_stack_t stack = rpl_context_get_stack(context);
    ck_assert_ptr_nonnull(stack);

    ck_assert_int_eq(2, rpl_stack_get_level(stack));

    rpl_value_t level_0 = rpl_stack_get_value_at_level(stack, 0);
    ck_assert_ptr_nonnull(level_0);
    ck_assert_int_eq(rpl_type_real, rpl_value_get_type(level_0));
    ck_assert_double_eq(123, rpl_real_get_rep(level_0));

    rpl_value_t level_1 = rpl_stack_get_value_at_level(stack, 1);
    ck_assert_ptr_nonnull(level_1);
    ck_assert_int_eq(rpl_type_real, rpl_value_get_type(level_1));
    ck_assert_double_eq(123, rpl_real_get_rep(level_1));
}
END_TEST

START_TEST(test_STO_and_RCL)
{
    const char * text_c = "123 'A' STO\n";
    const size_t text_c_len = strlen(text_c);
    rpl_unistring_t text = rpl_unistring_new_from_utf8(text_c,
						       text_c_len);
    ck_assert_ptr_nonnull(text);

    bool appended = rpl_interpreter_append_input(interpreter, text);
    ck_assert(appended);

    bool did_push_123 = rpl_interpreter_step(interpreter);
    ck_assert(did_push_123);

    bool did_push_name = rpl_interpreter_step(interpreter);
    ck_assert(did_push_name);

    bool did_STO = rpl_interpreter_step(interpreter);
    ck_assert(did_STO);

    rpl_context_t context = rpl_interpreter_get_context(interpreter);
    ck_assert_ptr_nonnull(context);

    rpl_scope_t scope = rpl_context_get_local_scope(context);
    ck_assert_ptr_nonnull(scope);

    rpl_unistring_t name = rpl_unistring_new_from_utf8("A", 1);
    ck_assert_ptr_nonnull(name);

    rpl_value_t value = rpl_scope_get_variable(scope, name, false);
    ck_assert_ptr_nonnull(value);
    ck_assert_int_eq(rpl_type_real, rpl_value_get_type(value));
    rpl_real_t value_rep = rpl_real_get_rep(value);
    ck_assert_double_eq(value_rep, 123);
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
    tcase_add_test(tc_interpreter, test_trivial_execution);
    tcase_add_test(tc_interpreter, test_STO_and_RCL);

    suite_add_tcase(s, tc_interpreter);

    return s;
}


RPL_SOURCE_END
