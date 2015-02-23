/*
  A map container.
 */

#ifndef SA_HTBL_H
#define SA_HTBL_H


#include <stdbool.h>

#include "list.h"


typedef void *sa_htbl_value;

typedef unsigned int (*sa_htbl_hash_fn)(sa_htbl_value val);
typedef _Bool (*sa_htbl_equals_fn)(sa_htbl_value val1, sa_htbl_value val2);

typedef struct sa_htbl {
  sa_list **store_lists;
  int bins_no;
  sa_htbl_hash_fn hash_fn;
  sa_htbl_equals_fn equals_fn;
} sa_htbl;


sa_htbl *sa_htbl_new(sa_htbl_hash_fn hash_fn, sa_htbl_equals_fn equals_fn);
void sa_htbl_free(sa_htbl *htbl);

_Bool sa_htbl_contains(sa_htbl *htbl, sa_htbl_value value);
int sa_htbl_add(sa_htbl *htbl, sa_htbl_value value);
sa_htbl_value sa_htbl_remove(sa_htbl *htbl, sa_htbl_value value);
int sa_htbl_count(sa_htbl *htbl);

/* debugging stuff */
void sa_htbl_print(sa_htbl *htbl);
void sa_htbl_print_bin_lengths(sa_htbl *htbl);

/* sa_htbl_iterator */

typedef struct sa_htbl_iterator {
  sa_htbl *htbl;
  int bin_no;
  sa_list_node *current;
} sa_htbl_iterator;

sa_htbl_iterator *sa_htbl_iterator_new(sa_htbl *htbl);
void sa_htbl_iterator_free(sa_htbl_iterator *iter);
sa_htbl_value sa_htbl_iterator_next(sa_htbl_iterator *iter);


#endif
