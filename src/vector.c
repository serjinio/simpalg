

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "vector.h"


#define INITIAL_CAPACITY 25

#define is_full(vector) ((vector)->length == (vector)->capacity)


/* private members */

static int
sa_vector_expand(sa_vector *vector) {
  assert(vector != NULL);

  unsigned int new_capacity = vector->capacity * 2;
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
  
  if (is_full(vector)) {
    int rc = sa_vector_expand(vector);
    if (rc != 0) {
      return rc;
    }
  }
  
  sa_vector_value *pval = vector->store + sa_vector_length(vector);
  *pval = value;
  vector->length += 1;
  return 0;
}

sa_vector_value
sa_vector_nth(sa_vector *vector, unsigned int n) {
  assert(vector != NULL && n < sa_vector_length(vector));
  return *(vector->store + n);
}

	      
sa_vector_value
sa_vector_remove_nth(sa_vector *vector, unsigned int index) {
  assert(vector != NULL && index < sa_vector_length(vector));

  sa_vector_value val = sa_vector_nth(vector, index);
  
  for (int i = index; i < sa_vector_length(vector); i++) {
    *(vector->store + i) = *(vector->store + (i + 1));
  }
  vector->length -= 1;

  return val;
}

