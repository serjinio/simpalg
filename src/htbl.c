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
#include "htbl.h"


#define SET_BINS_NO 50


#define htbl_bin(htbl, bin_no) ((htbl)->store_lists[bin_no])
#define value_bin(htbl, val) ((htbl)->hash_fn(val) % (htbl)->bins_no)


sa_htbl *
sa_htbl_new(sa_htbl_hash_fn hash_fn, sa_htbl_equals_fn equals_fn) {
  assert(hash_fn != NULL && equals_fn != NULL);
  
  sa_htbl *htbl = malloc(sizeof(sa_htbl));
  *htbl = (sa_htbl){.bins_no = SET_BINS_NO,
		  .equals_fn = equals_fn,
		  .hash_fn = hash_fn};
  
  htbl->store_lists = malloc(sizeof(sa_list *) * htbl->bins_no);    
  for (int i = 0; i < htbl->bins_no; i++) {
    sa_list **lst = &htbl_bin(htbl, i);
    *lst = sa_list_new((sa_list_equals_fn)equals_fn);
  }
  
  return htbl;
}

void
sa_htbl_free(sa_htbl *htbl) {
  assert(htbl != NULL);
  for (int i = 0; i < htbl->bins_no; i++) {
    sa_list *lst = htbl_bin(htbl, i);
    sa_list_free(lst);
  }
  free(htbl->store_lists);
  free(htbl);
}

int
sa_htbl_add(sa_htbl *htbl, sa_htbl_value value) {
  if (sa_htbl_contains(htbl, value)) {
    return 2;
  }
  int value_bin = value_bin(htbl, value);
  assert(value_bin < SET_BINS_NO);
  sa_list_append(htbl_bin(htbl, value_bin), value);
  return 0;
}

sa_htbl_value
sa_htbl_remove(sa_htbl *htbl, sa_htbl_value value) {
  assert(htbl != NULL);

  if (!sa_htbl_contains(htbl, value)) {
    return NULL;
  }

  int bin_no = value_bin(htbl, value);
  sa_list_node *node = sa_list_find(htbl_bin(htbl, bin_no), value);
  return sa_list_remove(htbl_bin(htbl, bin_no), node);
}

_Bool
sa_htbl_contains(sa_htbl *htbl, sa_htbl_value value) {
  int value_bin = value_bin(htbl, value);
  assert(value_bin < SET_BINS_NO);
  return sa_list_contains(htbl_bin(htbl, value_bin),
			      (sa_list_value)value);
}

int
sa_htbl_count(sa_htbl *htbl) {
  int count = 0;
  for (int i = 0; i < htbl->bins_no; i++) {
    count += sa_list_length(htbl_bin(htbl, i));
  }
  return count;
}


/* debugging functions */

void
sa_htbl_print(struct sa_htbl *htbl) {
  printf("printing htbl contents. htbl bins no.: %d\n", htbl->bins_no);
  for (int i = 0; i < htbl->bins_no; i++) {
    printf("contents of bin no.: %d\n:", i);
    sa_list_print(htbl_bin(htbl, i));
  }
}

void
sa_htbl_print_bin_lengths(struct sa_htbl *htbl) {
  for (int i = 0; i < htbl->bins_no; i++) {
    struct sa_list *lst = htbl_bin(htbl, i);
    printf("bin # %d length: %lu\n", i, sa_list_length(lst));
  }
}


/* htbl iterator */


#define iter_terminated(iter) ((iter)->bin_no == (iter)->htbl->bins_no)


sa_htbl_iterator *
sa_htbl_iterator_new(sa_htbl *htbl) {
  sa_htbl_iterator *iter = malloc(sizeof(sa_htbl_iterator));
  *iter = (sa_htbl_iterator){.htbl = htbl, .bin_no = -1, .current = NULL};
  return iter;
}

void
sa_htbl_iterator_free(sa_htbl_iterator *iter) {
  assert(iter != NULL);
  free(iter);
}

sa_htbl_value
sa_htbl_iterator_next(sa_htbl_iterator *iter) {
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

    iter->current = sa_list_head(htbl_bin(iter->htbl, iter->bin_no));
  }
  
  return sa_list_value(iter->current);
}
  
