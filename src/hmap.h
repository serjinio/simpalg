/*
  A map container.
 */

#ifndef SA_HMAP_H
#define SA_HMAP_H


#include <stdbool.h>

#include "list.h"


typedef void *sa_hmap_key;
typedef void *sa_hmap_value;

typedef struct sa_hmap_keyval {
  sa_hmap_key key;
  sa_hmap_value value;
} sa_hmap_keyval;

typedef unsigned int (*sa_hmap_hash_fn)(sa_hmap_key key);
typedef _Bool (*sa_hmap_equals_fn)(sa_hmap_key key1, sa_hmap_key key2);

typedef struct sa_hmap {
  sa_list **store_lists;
  int bins_no;
  sa_hmap_hash_fn hash_fn;
  sa_hmap_equals_fn equals_fn;
} sa_hmap;


sa_hmap *sa_hmap_new(sa_hmap_hash_fn hash_fn, sa_hmap_equals_fn equals_fn);
void sa_hmap_free(sa_hmap *hmap);

_Bool sa_hmap_contains(sa_hmap *hmap, sa_hmap_key key);
int sa_hmap_add(sa_hmap *hmap, sa_hmap_key key, sa_hmap_value value);
sa_hmap_keyval *sa_hmap_remove(sa_hmap *hmap, sa_hmap_key key);
int sa_hmap_count(sa_hmap *hmap);

/* debugging stuff */
void sa_hmap_print(sa_hmap *hmap);
void sa_hmap_print_bin_lengths(sa_hmap *hmap);

/* sa_hmap_iterator */

typedef struct sa_hmap_iterator {
  sa_hmap *hmap;
  int bin_no;
  sa_list_node *current;
} sa_hmap_iterator;

sa_hmap_iterator *sa_hmap_iterator_new(sa_hmap *hmap);
void sa_hmap_iterator_free(sa_hmap_iterator *iter);
sa_hmap_keyval *sa_hmap_iterator_next(sa_hmap_iterator *iter);


#endif
