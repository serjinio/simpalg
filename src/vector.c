

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "vector.h"


#define INITIAL_CAPACITY 25

#define is_full(vector) ((vector)->length == (vector)->capacity)
#define should_resize(vector) (sa_vector_length((vector)) < \
			       sa_vector_capacity((vector)) / 4)

/* private members */

static int
sa_vector_resize(sa_vector *vector) {
  assert(vector != NULL);

  unsigned int new_capacity = sa_vector_length(vector) * 2;
  size_t new_store_size = sizeof(sa_vector_value) * new_capacity;
  vector->store = realloc(vector->store, new_store_size);
  if (vector->store == NULL) {
    return -1;
  }
  vector->capacity = new_capacity;
  
  return 0;
}


/* public interface implementation */

sa_vector *
sa_vector_new(sa_vector_equals_fn equals_fn) {
  sa_vector_value vector_store =
    malloc(sizeof(sa_vector_value) * INITIAL_CAPACITY);
  if (vector_store == NULL) {
    return NULL;
  }
  
  sa_vector *vector = malloc(sizeof(sa_vector));
  if (vector == NULL) {
    return NULL;
  }
  *vector = (sa_vector){.capacity = INITIAL_CAPACITY, .length = 0,
			.equals_fn = equals_fn, .store = vector_store};
  
  return vector;
}

void
sa_vector_free(sa_vector *vector) {
  assert(vector != NULL);

  free(vector->store);
  free(vector);
}

int
sa_vector_add(sa_vector *vector, sa_vector_value value) {
  assert(vector != NULL);

  if (is_full(vector) && sa_vector_resize(vector) != 0)
    return -1;
  
  sa_vector_value *pval = vector->store + sa_vector_length(vector);
  *pval = value;
  vector->length += 1;
  
  return 0;
}

int
sa_vector_nth(sa_vector *vector, unsigned int n, sa_vector_value *pvalue) {
  assert(vector != NULL && n < sa_vector_length(vector));
  *pvalue = *(vector->store + n);
  return 0;
}

int sa_vector_set_nth(sa_vector *vector, unsigned int n, sa_vector_value pvalue) {
  assert(vector != NULL && n < sa_vector_length(vector));
  vector->store[n] = pvalue;
  return 0;
}

int
sa_vector_remove_nth(sa_vector *vector, unsigned int n,
		     sa_vector_value *pvalue) {
  assert(vector != NULL && n < sa_vector_length(vector));

  if (sa_vector_nth(vector, n, pvalue) != 0) {
    return 1;
  }
  
  for (int i = n; i < sa_vector_length(vector); i++) {
    *(vector->store + i) = *(vector->store + (i + 1));
  }
  vector->length -= 1;

  if (should_resize(vector)) {
    int rc = sa_vector_resize(vector);
    if (rc != 0)
      return -1;
  }

  return 0;
}

long
sa_vector_indexof(sa_vector *vector, sa_vector_value value) {
  assert(vector != NULL && vector->equals_fn != NULL);

  if (sa_vector_length(vector) == 0)
    return -1;

  sa_vector_value vect_value;
  if (sa_vector_nth(vector, 0, &vect_value) != 0) {
    return -1;
  }
  
  int idx = 0;
  _Bool found = false;
  while(idx < sa_vector_length(vector)) {
    if (vector->equals_fn(vect_value, value) == true) {
      found = true;
      break;
    }
    idx += 1;
  }
  
  if (found == true) {
    return idx;
  } else {
    return -1;
  }
}

int
sa_vector_swap(sa_vector *vector, unsigned int n, unsigned int k) {
  assert(vector != NULL && n < sa_vector_length(vector) &&
	 k < sa_vector_length(vector));

  sa_vector_value tmp = vector->store[n];
  vector->store[n] = vector->store[k];
  vector->store[k] = tmp;
  
  return 0;
}
