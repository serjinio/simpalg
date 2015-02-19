/*
  A map container.
 */

#ifndef SA_MAP_H
#define SA_MAP_H


#include <stdbool.h>

#include "list.h"


typedef void *sa_set_value;

typedef unsigned int (*sa_set_hash_fn)(sa_set_value val);
typedef _Bool (*sa_set_equals_fn)(sa_set_value val1, sa_set_value val2);

typedef struct sa_set {
  sa_list **store_lists;
  int bins_no;
  sa_set_hash_fn hash_fn;
  sa_set_equals_fn equals_fn;
} sa_set;

/* sa_set */
  
sa_set *sa_set_new(sa_set_hash_fn hash_fn, sa_set_equals_fn equals_fn);
void sa_set_free(sa_set *map);

_Bool sa_set_contains(sa_set *map, sa_set_value value);
int sa_set_put(sa_set *map, sa_set_value value);
sa_set_value sa_set_remove(sa_set *map, sa_set_value value);
int sa_set_count(sa_set *map);

/** debugging stuff **/
void sa_set_print(sa_set *map);
void sa_set_print_bin_lengths(sa_set *map);

/* sa_set_iterator */

typedef struct sa_set_iterator {
  sa_set *map;
  int bin_no;
  sa_list_node *current;
} sa_set_iterator;

sa_set_iterator *sa_set_iterator_new(sa_set *map);
void sa_set_iterator_free(sa_set_iterator *iter);
sa_set_value sa_set_iterator_next(sa_set_iterator *iter);


#endif
