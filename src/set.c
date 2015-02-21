/**
   Sample of a set container
**/

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

#include "list.h"
#include "set.h"


#define SET_BINS_NO 50


#define set_bin(set, bin_no) ((set)->store_lists[bin_no])
#define value_bin(set, val) ((set)->hash_fn(val) % (set)->bins_no)


sa_set *
sa_set_new(sa_set_hash_fn hash_fn, sa_set_equals_fn equals_fn) {
  assert(hash_fn != NULL && equals_fn != NULL);
  
  sa_set *set = malloc(sizeof(sa_set));
  *set = (sa_set){.bins_no = SET_BINS_NO,
		  .equals_fn = equals_fn,
		  .hash_fn = hash_fn};
  
  set->store_lists = malloc(sizeof(sa_list *) * set->bins_no);    
  for (int i = 0; i < set->bins_no; i++) {
    sa_list **lst = &set_bin(set, i);
    *lst = sa_list_new((sa_list_equals_fn)equals_fn);
  }
  
  return set;
}

void
sa_set_free(sa_set *set) {
  assert(set != NULL);
  for (int i = 0; i < set->bins_no; i++) {
    sa_list *lst = set_bin(set, i);
    sa_list_free(lst);
  }
  free(set->store_lists);
  free(set);
}

int
sa_set_add(sa_set *set, sa_set_value value) {
  if (sa_set_contains(set, value)) {
    return 2;
  }
  int value_bin = value_bin(set, value);
  assert(value_bin < SET_BINS_NO);
  sa_list_append(set_bin(set, value_bin), value);
  return 0;
}

sa_set_value
sa_set_remove(sa_set *set, sa_set_value value) {
  assert(set != NULL);

  if (!sa_set_contains(set, value)) {
    return NULL;
  }

  int bin_no = value_bin(set, value);
  sa_list_node *node = sa_list_find(set_bin(set, bin_no), value);
  return sa_list_remove(set_bin(set, bin_no), node);
}

_Bool
sa_set_contains(sa_set *set, sa_set_value value) {
  int value_bin = value_bin(set, value);
  assert(value_bin < SET_BINS_NO);
  return sa_list_contains(set_bin(set, value_bin),
			      (sa_list_value)value);
}

int
sa_set_count(sa_set *set) {
  int count = 0;
  for (int i = 0; i < set->bins_no; i++) {
    count += sa_list_length(set_bin(set, i));
  }
  return count;
}


/* debugging functions */

void
sa_set_print(struct sa_set *set) {
  printf("printing set contents. set bins no.: %d\n", set->bins_no);
  for (int i = 0; i < set->bins_no; i++) {
    printf("contents of bin no.: %d\n:", i);
    sa_list_print(set_bin(set, i));
  }
}

void
sa_set_print_bin_lengths(struct sa_set *set) {
  for (int i = 0; i < set->bins_no; i++) {
    struct sa_list *lst = set_bin(set, i);
    printf("bin # %d length: %lu\n", i, sa_list_length(lst));
  }
}


/* set iterator */


#define iter_terminated(iter) ((iter)->bin_no == (iter)->set->bins_no)


sa_set_iterator *
sa_set_iterator_new(sa_set *set) {
  sa_set_iterator *iter = malloc(sizeof(sa_set_iterator));
  *iter = (sa_set_iterator){.set = set, .bin_no = -1, .current = NULL};
  return iter;
}

void
sa_set_iterator_free(sa_set_iterator *iter) {
  assert(iter != NULL);
  free(iter);
}

sa_set_value
sa_set_iterator_next(sa_set_iterator *iter) {
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

    iter->current = sa_list_head(set_bin(iter->set, iter->bin_no));
  }
  
  return sa_list_value(iter->current);
}
  
