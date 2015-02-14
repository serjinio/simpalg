/*
  A map container.
 */

#ifndef SA_MAP_H
#define SA_MAP_H


#include <stdbool.h>

#include "list.h"


typedef void *sa_map_value;

typedef unsigned int (*sa_map_hash_fn)(sa_map_value val);
typedef _Bool (*sa_map_equals_fn)(sa_map_value val1, sa_map_value val2);

typedef struct sa_map {
  sa_list **store_lists;
  int bins_no;
  sa_map_hash_fn hash_fn;
  sa_map_equals_fn equals_fn;
} sa_map;

/* sa_map */
  
sa_map *sa_map_new(sa_map_hash_fn hash_fn, sa_map_equals_fn equals_fn);
void sa_map_free(sa_map *map);

_Bool sa_map_contains(sa_map *map, sa_map_value value);
int sa_map_put(sa_map *map, sa_map_value value);
sa_map_value sa_map_remove(sa_map *map, sa_map_value value);
int sa_map_count(sa_map *map);

/** debugging stuff **/
void sa_map_print(sa_map *map);
void sa_map_print_bin_lengths(sa_map *map);

/* sa_map_iterator */

typedef struct sa_map_iterator {
  sa_map *map;
  int bin_no;
  sa_list_node *current;
} sa_map_iterator;

sa_map_iterator *sa_map_iterator_new(sa_map *map);
void sa_map_iterator_free(sa_map_iterator *iter);
sa_map_value sa_map_iterator_next(sa_map_iterator *iter);


#endif
