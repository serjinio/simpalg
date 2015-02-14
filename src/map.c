/**
   Sample of a map store
**/

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

#include "list.h"
#include "map.h"


#define MAP_BINS_NO 50


#define map_bin(map, bin_no) ((map)->store_lists[bin_no])
#define value_bin(map, val) ((map)->hash_fn(val) % (map)->bins_no)


struct sa_map *
sa_map_new(sa_map_hash_fn hash_fn, sa_map_equals_fn equals_fn) {
  assert(hash_fn != NULL && equals_fn != NULL);
  
  sa_map *map = malloc(sizeof(sa_map));
  *map = (sa_map){.bins_no = MAP_BINS_NO,
		  .equals_fn = equals_fn,
		  .hash_fn = hash_fn};
  
  map->store_lists = malloc(sizeof(sa_list *) * map->bins_no);    
  for (int i = 0; i < map->bins_no; i++) {
    sa_list **lst = &map_bin(map, i);
    *lst = sa_list_new((sa_list_equals_fn)equals_fn);
  }
  
  return map;
}

void
sa_map_free(sa_map *map) {
  assert(map != NULL);
  for (int i = 0; i < map->bins_no; i++) {
    sa_list *lst = map_bin(map, i);
    sa_list_free(lst);
  }
  free(map->store_lists);
  free(map);
}

int
sa_map_put(sa_map *map, sa_map_value value) {
  if (sa_map_contains(map, value)) {
    return -2;
  }
  int value_bin = value_bin(map, value);
  assert(value_bin < MAP_BINS_NO);
  sa_list_append(map_bin(map, value_bin), value);
  return 0;
}

sa_map_value
sa_map_remove(sa_map *map, sa_map_value value) {
  assert(map != NULL);

  if (!sa_map_contains(map, value)) {
    return NULL;
  }

  int bin_no = value_bin(map, value);
  sa_list_node *node = sa_list_find(map_bin(map, bin_no), value);
  return sa_list_remove(map_bin(map, bin_no), node);
}

_Bool
sa_map_contains(sa_map *map, sa_map_value value) {
  int value_bin = value_bin(map, value);
  assert(value_bin < MAP_BINS_NO);
  return sa_list_contains(map_bin(map, value_bin),
			      (sa_list_value)value);
}

int
sa_map_count(sa_map *map) {
  int count = 0;
  for (int i = 0; i < map->bins_no; i++) {
    count += sa_list_length(map_bin(map, i));
  }
  return count;
}

void
sa_map_print(struct sa_map *map) {
  printf("printing map contents. map bins no.: %d\n", map->bins_no);
  for (int i = 0; i < map->bins_no; i++) {
    printf("contents of bin no.: %d\n:", i);
    sa_list_print(map_bin(map, i));
  }
}

void
sa_map_print_bin_lengths(struct sa_map *map) {
  for (int i = 0; i < map->bins_no; i++) {
    struct sa_list *lst = map_bin(map, i);
    printf("bin # %d length: %lu\n", i, sa_list_length(lst));
  }
}


/* map iterator */


#define iter_terminated(iter) (iter->bin_no == iter->map->bins_no)


sa_map_iterator *
sa_map_iterator_new(sa_map *map) {
  sa_map_iterator *iter = malloc(sizeof(sa_map_iterator));
  *iter = (sa_map_iterator){.map = map, .bin_no = -1, .current = NULL};
  return iter;
}

void
sa_map_iterator_free(sa_map_iterator *iter) {
  assert(iter != NULL);
  free(iter);
}

sa_map_value
sa_map_iterator_next(sa_map_iterator *iter) {
  assert(iter != NULL);
  
  if (iter_terminated(iter)) {
    return NULL;
  }

  if (iter->current != NULL) {
    iter->current = sa_list_next(iter->current);
  }

  // if we got past the end of the current bin, then advance
  while (iter->current == NULL && !iter_terminated(iter)) {
    iter->bin_no += 1;
    if (iter_terminated(iter)) {
      return NULL;
    }

    iter->current = sa_list_head(map_bin(iter->map, iter->bin_no));
  }
  
  return sa_list_value(iter->current);
}
  
