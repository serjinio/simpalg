/*
  Tests for hash container
*/

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

#include <check.h>

#include <set.h>
#include <list.h>
#include <vector.h>


static char *
rand_str(char *str, size_t length) {
  static int seed = 3457;
  const char * const charset =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";

  if (str == NULL)
    return NULL;
      
  srand(time(NULL) * length + seed);
  seed += 2;
  length--;
  for (int i = 0; i < length; i++) {
    int key = rand() % (int)(sizeof(charset) - 1);
    str[i] = charset[key];
  }
  str[length] = '\0';
    
  
  //printf("generated random string: %s\n", str);
  return str;
}


static inline char *
rand_str_alloc(size_t length) {
  return length ? rand_str(malloc(sizeof(char) * length), length) : NULL;
}

_Bool str_equals_fn(sa_list_value val1, sa_list_value val2) {
  if (strcmp((char *)val1, (char *)val2) == 0) {
    return true;
  } else {
    return false;
  }
}

unsigned int
hash_string(sa_set_value val)
{
  unsigned char *str = (unsigned char *)val;
  unsigned long hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}


START_TEST(test_set_basic)
{
  struct sa_set *set = sa_set_new(hash_string, str_equals_fn);
  char *val;
  asprintf(&val, "%s", "sample value one");
  
  printf("test add one...\n");
  sa_set_put(set, val);
  ck_assert_int_eq(sa_set_count(set), 1);
  ck_assert(sa_set_contains(set, val) == true);
  ck_assert(sa_set_contains(set, "bad value") == false);

  printf("adding 99 more...\n");
  for (int i = 0; i < 99; i++) {
    val = rand_str_alloc(50);
    sa_set_put(set, val);
  }
  ck_assert(sa_set_count(set) == 100);
}
END_TEST

START_TEST(test_set_remove) {
  struct sa_set *set = sa_set_new(hash_string, str_equals_fn);
  char *val;
  asprintf(&val, "%s", "sample value");

  sa_set_put(set, val);
  ck_assert(sa_set_contains(set, val) == true);
  sa_set_remove(set, val);
  ck_assert(sa_set_contains(set, val) == false);
  ck_assert(sa_set_count(set) == 0);
}
END_TEST

START_TEST(test_set_iterate) {
  sa_set *set = sa_set_new(hash_string, str_equals_fn);
  char *val;

  for (int i = 0; i < 50; i++) {
    val = rand_str_alloc(50);
    sa_set_put(set, val);
  }
  
  int counter = 0;
  sa_set_iterator *iter = sa_set_iterator_new(set);
  sa_set_value set_val = NULL;
  while ((set_val = sa_set_iterator_next(iter))) {
    counter += 1;
  }
  sa_set_iterator_free(iter);
  ck_assert(counter == 50);
}
END_TEST

START_TEST(test_set_perf)
{
  struct sa_set *set = sa_set_new(hash_string, str_equals_fn);
  char *val;

  printf("test add many items...\n");
  for (int i = 0; i < 10000; i++) {
    val = rand_str_alloc(50);
    sa_set_put(set, val);
  }
  printf("added all elements: %d\n", sa_set_count(set));
  ck_assert(sa_set_count(set) == 10000);
  printf("passed.\n");

}
END_TEST

START_TEST(test_list_basic) {
  sa_list *lst = sa_list_new(str_equals_fn);
  char *val1, *val2;
  
  asprintf(&val1, "%s", "sample value one");
  asprintf(&val2, "%s", "sample value two");
  
  sa_list_append(lst, val1);
  ck_assert(lst->length == 1);
  sa_list_prepend(lst, val2);
  ck_assert(lst->length == 2);
  
  ck_assert(strcmp(sa_list_head(lst)->value, val2) == 0);
  ck_assert(strcmp(sa_list_tail(lst)->value, val1) == 0);
  
  ck_assert(sa_list_find(lst, val1) != NULL);
  ck_assert(sa_list_find(lst, val2) != NULL);
  ck_assert(sa_list_find(lst, "bad value") == NULL);
  
  sa_list_remove(lst, sa_list_head(lst));
  ck_assert(lst->length == 1);
  sa_list_remove(lst, sa_list_tail(lst));
  ck_assert(lst->length == 0);
}
END_TEST

START_TEST(test_vector_constructor) {
  sa_vector *vec = sa_vector_new(NULL);
  ck_assert(sa_vector_length(vec) == 0);
  sa_vector_free(vec);
}
END_TEST

Suite *hash_suite(void)
{
  Suite *s;
  TCase *tc_core;

  s = suite_create("Hash Set");

  /* Core test case */
  tc_core = tcase_create("Basic");

  tcase_add_test(tc_core, test_set_perf);
  tcase_add_test(tc_core, test_set_remove);
  tcase_add_test(tc_core, test_set_iterate);
  tcase_add_test(tc_core, test_set_basic);
  suite_add_tcase(s, tc_core);

  return s;
}

Suite *list_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("List");

  /* Core test case */
  tc_core = tcase_create("Basic");

  tcase_add_test(tc_core, test_list_basic);
  suite_add_tcase(s, tc_core);

  return s;
}

Suite *vector_suite(void)
{
  Suite *s;
  TCase *tc_core;

  s = suite_create("Vector");

  /* Core test case */
  tc_core = tcase_create("Basic");

  tcase_add_test(tc_core, test_vector_constructor);
  suite_add_tcase(s, tc_core);
  
  return s;
}

int run_suite(Suite *suite) {
  int number_failed;
  SRunner *sr_suite = srunner_create(suite);
  srunner_run_all(sr_suite, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr_suite);
  srunner_free(sr_suite);
  return number_failed;
}

int main(void)
{
  int number_failed = 0;
  Suite *hash_set, *list, *vect;

  hash_set = hash_suite();
  number_failed += run_suite(hash_set);

  list = list_suite();
  number_failed += run_suite(list);

  vect = vector_suite();
  number_failed += run_suite(vect);
  
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
