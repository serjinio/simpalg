/*
  A resizable array container.
*/

#ifndef SA_VECTOR_H
#define SA_VECTOR_H


#include <stdbool.h>


typedef void *sa_vector_value;

typedef _Bool (*sa_vector_equals_fn)(sa_vector_value val1, sa_vector_value val2);


typedef struct sa_vector {
  sa_vector_value store;
  unsigned int capacity;
  unsigned int length;
  sa_vector_equals_fn equals_fn;
} sa_vector;


sa_vector *sa_vector_new(sa_vector_equals_fn equals_fn);
void sa_vector_free(sa_vector *vector);

int sa_vector_add(sa_vector *vector, sa_vector_value value);
int sa_vector_remove_nth(sa_vector *vector, unsigned int index);
_Bool sa_vector_remove_value(sa_vector *vector, sa_vector_value value);
sa_vector_value sa_vector_nth(unsigned int n);

unsigned int sa_vector_indexof(sa_vector *vector, sa_vector_value);

#define sa_vector_length(vector) ((vector)->length)
#define sa_vector_contains(vector, value) (sa_vector_indexof((vector), (value)) != -1)


#endif
