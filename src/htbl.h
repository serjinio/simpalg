/*
  A map container.
 */

#ifndef SA_HTBL_H
#define SA_HTBL_H


#include <stdbool.h>

#include "hmap.h"


#define sa_htbl sa_hmap
#define sa_htbl_value sa_hmap_key

#define sa_htbl_new sa_hmap_new
#define sa_htbl_free sa_hmap_free
#define sa_htbl_count sa_hmap_count

_Bool sa_htbl_contains(sa_htbl *htbl, sa_htbl_value value);
int sa_htbl_add(sa_htbl *htbl, sa_htbl_value value);
sa_htbl_value sa_htbl_remove(sa_htbl *htbl, sa_htbl_value value);

/* iterator */

#define sa_htbl_iterator sa_hmap_iterator

#define sa_htbl_iterator_new sa_hmap_iterator_new
#define sa_htbl_iterator_free sa_hmap_iterator_free

sa_htbl_value *sa_htbl_iterator_next(sa_htbl_iterator *iter);

#endif
