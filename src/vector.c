

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "vector.h"


#define INITIAL_CAPACITY 50


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
