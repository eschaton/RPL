//
//  test_adjbuffer.c
//  RPL
//
//  Created by Chris Hanson on 8/29/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#include "test_adjbuffer.h"

#include "rpl_adjbuffer.h"


RPL_SOURCE_BEGIN


struct test_element {
    int a;
    int b;
};
typedef struct test_element test_element_t;

bool test_element_is_equal(const test_element_t *a,
			   const test_element_t *b)
{
    return memcmp(a, b, sizeof(test_element_t)) == 0;
}


void
test_adjbuffer_setup(void)
{
    tests_shared_setup();

    /* Add other setup here. */
}


void
test_adjbuffer_teardown(void)
{
    /* Add other teardown here. */

    tests_shared_teardown();
}


/* MARK: - Tests */

START_TEST(test_creation)
{
    rpl_adjbuffer_t *ab = rpl_adjbuffer_new(0, sizeof(test_element_t));
    ck_assert_ptr_nonnull(ab);
    ck_assert_ptr_nonnull(ab->_storage);
    ck_assert_int_eq(ab->_count, 0);
    ck_assert_int_eq(ab->_capacity, 8);
    ck_assert_int_eq(ab->_element_size, sizeof(test_element_t));
}
END_TEST

START_TEST(test_insertion)
{
    rpl_adjbuffer_t *ab = rpl_adjbuffer_new(0, sizeof(test_element_t));
    ck_assert_ptr_nonnull(ab);

    test_element_t te1 = { 0, 1 },
		   te2 = { 2, 3 },
		   te3 = { 4, 5 };

    /* Construct an initial adjbuffer of [te1, te3]. */

    bool appended_te1 = rpl_adjbuffer_append(ab, &te1);
    ck_assert(appended_te1);
    bool appended_te3 = rpl_adjbuffer_append(ab, &te3);
    ck_assert(appended_te3);

    ck_assert_int_eq(rpl_adjbuffer_get_count(ab), 2);

    /* Ensure the adjbuffer contains what we think it does. */

    test_element_t *tep = NULL;

    tep = rpl_adjbuffer_get(ab, 0);
    ck_assert_ptr_nonnull(tep);
    ck_assert(test_element_is_equal(tep, &te1));

    tep = rpl_adjbuffer_get(ab, 1);
    ck_assert_ptr_nonnull(tep);
    ck_assert(test_element_is_equal(tep, &te3));

    /* Now check whether insertion works. */

    bool inserted_te2 = rpl_adjbuffer_insert(ab, 1, &te2);
    ck_assert(inserted_te2);

    ck_assert_int_eq(rpl_adjbuffer_get_count(ab), 3);

    tep = rpl_adjbuffer_get(ab, 0);
    ck_assert_ptr_nonnull(tep);
    ck_assert(test_element_is_equal(tep, &te1));

    tep = rpl_adjbuffer_get(ab, 1);
    ck_assert_ptr_nonnull(tep);
    ck_assert(test_element_is_equal(tep, &te2));

    tep = rpl_adjbuffer_get(ab, 2);
    ck_assert_ptr_nonnull(tep);
    ck_assert(test_element_is_equal(tep, &te3));
}
END_TEST

START_TEST(test_removal)
{
    rpl_adjbuffer_t *ab = rpl_adjbuffer_new(0, sizeof(test_element_t));
    ck_assert_ptr_nonnull(ab);

    test_element_t te1 = { 0, 1 },
		   te2 = { 2, 3 };

    /* Construct an initial adjbuffer of [te1, te3]. */

    bool appended_te1 = rpl_adjbuffer_append(ab, &te1);
    ck_assert(appended_te1);
    bool appended_te2 = rpl_adjbuffer_append(ab, &te2);
    ck_assert(appended_te2);

    ck_assert_int_eq(rpl_adjbuffer_get_count(ab), 2);

    /* Ensure the adjbuffer contains what we think it does. */

    test_element_t *tep = NULL;

    tep = rpl_adjbuffer_get(ab, 0);
    ck_assert_ptr_nonnull(tep);
    ck_assert(test_element_is_equal(tep, &te1));

    tep = rpl_adjbuffer_get(ab, 1);
    ck_assert_ptr_nonnull(tep);
    ck_assert(test_element_is_equal(tep, &te2));

    /* Now check whether removal works. */

    rpl_adjbuffer_remove(ab, 0);
    ck_assert_int_eq(rpl_adjbuffer_get_count(ab), 1);

    tep = rpl_adjbuffer_get(ab, 0);
    ck_assert_ptr_nonnull(tep);
    ck_assert(test_element_is_equal(tep, &te2));
}
END_TEST


/* MARK: - Test Infrastructure */

Suite *
test_adjbuffer_suite(void)
{
    Suite *s = suite_create("test_adjbuffer");

    TCase *tc_adjbuffer = tcase_create("test_adjbuffer");
    tcase_add_checked_fixture(tc_adjbuffer,
			      test_adjbuffer_setup,
			      test_adjbuffer_teardown);
    tcase_add_test(tc_adjbuffer, test_creation);
    tcase_add_test(tc_adjbuffer, test_insertion);
    tcase_add_test(tc_adjbuffer, test_removal);

    suite_add_tcase(s, tc_adjbuffer);

    return s;
}


RPL_SOURCE_END
