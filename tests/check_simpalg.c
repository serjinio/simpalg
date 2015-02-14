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

#include <map.h>
#include <list.h>


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
hash_string(sa_map_value val)
{
  unsigned char *str = (unsigned char *)val;
  unsigned long hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}


START_TEST(test_map_basic)
{
  struct sa_map *map = sa_map_new(hash_string, str_equals_fn);
  char *val;
  asprintf(&val, "%s", "sample value one");
  
  printf("test add one...\n");
  sa_map_put(map, val);
  ck_assert_int_eq(sa_map_count(map), 1);
  ck_assert(sa_map_contains(map, val) == true);
  ck_assert(sa_map_contains(map, "bad value") == false);

  printf("adding 99 more...\n");
  for (int i = 0; i < 99; i++) {
    val = rand_str_alloc(50);
    sa_map_put(map, val);
  }
  ck_assert(sa_map_count(map) == 100);
}
END_TEST

START_TEST(test_map_remove) {
  struct sa_map *map = sa_map_new(hash_string, str_equals_fn);
  char *val;
  asprintf(&val, "%s", "sample value");

  sa_map_put(map, val);
  ck_assert(sa_map_contains(map, val) == true);
  sa_map_remove(map, val);
  ck_assert(sa_map_contains(map, val) == false);
  ck_assert(sa_map_count(map) == 0);
}
END_TEST

START_TEST(test_map_iterate) {
  sa_map *map = sa_map_new(hash_string, str_equals_fn);
  char *val;

  for (int i = 0; i < 50; i++) {
    val = rand_str_alloc(50);
    sa_map_put(map, val);
  }
  sa_map_print_bin_lengths(map);
  
  int counter = 0;
  sa_map_iterator *iter = sa_map_iterator_new(map);
  sa_map_value map_val = NULL;
  while ((map_val = sa_map_iterator_next(iter))) {
    counter += 1;
  }
  sa_map_iterator_free(iter);
  ck_assert(counter == 50);
}
END_TEST

START_TEST(test_map_perf)
{
  struct sa_map *map = sa_map_new(hash_string, str_equals_fn);
  char *val;

  printf("test add many items...\n");
  for (int i = 0; i < 10000; i++) {
    val = rand_str_alloc(50);
    sa_map_put(map, val);
  }
  printf("added all elements: %d\n", sa_map_count(map));
  ck_assert(sa_map_count(map) == 10000);
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

Suite *hash_suite(void)
{
  Suite *s;
  TCase *tc_core;

  s = suite_create("Hash");

  /* Core test case */
  tc_core = tcase_create("Basic");

  tcase_add_test(tc_core, test_map_perf);
  tcase_add_test(tc_core, test_map_remove);
  tcase_add_test(tc_core, test_map_iterate);
  tcase_add_test(tc_core, test_map_basic);
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
  Suite *hash_map, *list;

  hash_map = hash_suite();
  number_failed += run_suite(hash_map);

  list = list_suite();
  number_failed += run_suite(list);
  
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
