/*
   Sample of a set container
*/

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

#include "list.h"
#include "hmap.h"

/* Private members */

#define BINS_NO 50


#define hmap_bin(hmap, bin_no) ((hmap)->store_lists[bin_no])
#define key_bin(hmap, key) ((hmap)->hash_fn(key) % (hmap)->bins_no)


sa_hmap_keyval *sa_hmap_keyval_new(sa_hmap_key key, sa_hmap_value value) {
  sa_hmap_keyval *keyval = malloc(sizeof(sa_hmap_keyval));
  *keyval = (sa_hmap_keyval){.key = key, .value = value};
  return keyval;
}

void sa_hmap_keyval_free(sa_hmap_keyval *keyval) {
  free(keyval);
}

sa_list_node *
sa_hmap_list_find(sa_list *lst, sa_hmap_equals_fn equals_fn,
		  sa_hmap_key key) {
  assert(lst != NULL && equals_fn != NULL);

  sa_list_node *lst_node = sa_list_head(lst);
  while (lst_node != NULL) {
    sa_hmap_key lst_key = ((sa_hmap_keyval*)sa_list_value(lst_node))->key;
    if (equals_fn(lst_key, key) == true) {
      return lst_node;
    }
    lst_node = sa_list_next(lst_node);
  }

  return NULL;
}

#define sa_hmap_list_contains(lst, equals_fn, key)		\
  (sa_hmap_list_find((lst), (equals_fn), (key)) != NULL)

/* Public interface */

sa_hmap *
sa_hmap_new(sa_hmap_hash_fn hash_fn, sa_hmap_equals_fn equals_fn) {
  assert(hash_fn != NULL && equals_fn != NULL);
  
  sa_hmap *hmap = malloc(sizeof(sa_hmap));
  *hmap = (sa_hmap){.bins_no = BINS_NO,
		  .equals_fn = equals_fn,
		  .hash_fn = hash_fn};
  
  hmap->store_lists = malloc(sizeof(sa_list *) * hmap->bins_no);    
  for (int i = 0; i < hmap->bins_no; i++) {
    sa_list **lst = &hmap_bin(hmap, i);
    *lst = sa_list_new(NULL);
  }
  
  return hmap;
}

void
sa_hmap_free(sa_hmap *hmap) {
  assert(hmap != NULL);
  for (int i = 0; i < hmap->bins_no; i++) {
    sa_list *lst = hmap_bin(hmap, i);
    sa_list_free(lst);
  }
  free(hmap->store_lists);
  free(hmap);
}

int
sa_hmap_add(sa_hmap *hmap, sa_hmap_key key, sa_hmap_value value) {  
  if (sa_hmap_contains(hmap, key)) {
    return 2;
  }
  sa_hmap_keyval *keyval = sa_hmap_keyval_new(key, value);
  int bin_no = key_bin(hmap, key);
  assert(bin_no < BINS_NO);
  sa_list_append(hmap_bin(hmap, bin_no), keyval);
  return 0;
}

sa_hmap_keyval *
sa_hmap_remove(sa_hmap *hmap, sa_hmap_key key) {
  assert(hmap != NULL);

  if (!sa_hmap_contains(hmap, key)) {
    return NULL;
  }

  int bin_no = key_bin(hmap, key);
  sa_list_node *node = sa_hmap_list_find(hmap_bin(hmap, bin_no),
					 hmap->equals_fn, key);
  return (sa_hmap_keyval *)sa_list_remove(hmap_bin(hmap, bin_no), node);
}

_Bool
sa_hmap_contains(sa_hmap *hmap, sa_hmap_key key) {
  int bin_no = key_bin(hmap, key);
  assert(bin_no < BINS_NO);
  return sa_hmap_list_contains(hmap_bin(hmap, bin_no),
			       hmap->equals_fn, key);
}

int
sa_hmap_count(sa_hmap *hmap) {
  int count = 0;
  for (int i = 0; i < hmap->bins_no; i++) {
    count += sa_list_length(hmap_bin(hmap, i));
  }
  return count;
}


/* debugging functions */

void
sa_hmap_print(struct sa_hmap *hmap) {
  printf("printing hmap contents. hmap bins no.: %d\n", hmap->bins_no);
  for (int i = 0; i < hmap->bins_no; i++) {
    printf("contents of bin no.: %d\n:", i);
    sa_list_print(hmap_bin(hmap, i));
  }
}

void
sa_hmap_print_bin_lengths(struct sa_hmap *hmap) {
  for (int i = 0; i < hmap->bins_no; i++) {
    struct sa_list *lst = hmap_bin(hmap, i);
    printf("bin # %d length: %lu\n", i, sa_list_length(lst));
  }
}


/* hmap iterator */


#define iter_terminated(iter) ((iter)->bin_no == (iter)->hmap->bins_no)


sa_hmap_iterator *
sa_hmap_iterator_new(sa_hmap *hmap) {
  sa_hmap_iterator *iter = malloc(sizeof(sa_hmap_iterator));
  *iter = (sa_hmap_iterator){.hmap = hmap, .bin_no = -1, .current = NULL};
  return iter;
}

void
sa_hmap_iterator_free(sa_hmap_iterator *iter) {
  assert(iter != NULL);
  free(iter);
}

sa_hmap_keyval *
sa_hmap_iterator_next(sa_hmap_iterator *iter) {
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

    iter->current = sa_list_head(hmap_bin(iter->hmap, iter->bin_no));
  }
  
  return sa_list_value(iter->current);
}
  
