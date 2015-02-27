/*
  Heap container.
 */

#ifndef SA_HEAP_H
#define SA_HEAP_H


#include "vector.h"


typedef void * sa_heap_value;


typedef int (*sa_heap_compare_fn)(sa_heap_value val1, sa_heap_value val2);

				  
typedef struct sa_heap {
  sa_vector *store;
  sa_heap_compare_fn compare_fn;
} sa_heap;


sa_heap *sa_heap_new(sa_heap_compare_fn compare_fn);
void sa_heap_free(sa_heap *heap);

int sa_heap_put(sa_heap *heap, sa_heap_value value);
int sa_heap_extract(sa_heap *heap, sa_heap_value *value);


#define sa_heap_size(heap) (sa_vector_length((heap)->store))

#endif 
