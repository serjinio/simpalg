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

#include "htbl.h"


_Bool
sa_htbl_contains(sa_htbl *htbl, sa_htbl_value value) {
  return sa_hmap_contains(htbl, value);
}

int
sa_htbl_add(sa_htbl *htbl, sa_htbl_value value) {
  return sa_hmap_add(htbl, value, NULL);
}

sa_htbl_value
sa_htbl_remove(sa_htbl *htbl, sa_htbl_value value) {
  return sa_hmap_remove(htbl, value);
}

/* iterator */

sa_htbl_value *
sa_htbl_iter_next(sa_htbl_iter *iter) {
  sa_hmap_keyval *keyval = sa_hmap_iter_next(iter);
  if (keyval == NULL) {
    return NULL;
  }
  
  return (sa_htbl_value)keyval->key;
}
