

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "vector.h"
#include "heap.h"


/* private members */

#define left_index(i) ((((i) + 1) << 1) - 1)
#define right_index(i) (((i) + 1) << 1)
#define parent_index(i) ((((i) + 1) >> 1) - 1)
#define has_left(heap, i) (sa_heap_size((heap)) > left_index((i)))
#define has_right(heap, i) (sa_heap_size((heap)) > right_index((i)))
#define has_parent(heap, i) (sa_heap_size((heap)) > i && (i) > 0)

static sa_heap_value
sa_heap_nth(sa_heap *heap, int idx) {
  assert(idx < sa_heap_size(heap));
  sa_heap_value val;
  sa_vector_nth(heap->store, idx, ((sa_vector_value *)&val));
  return val;
}

#define sa_heap_left(heap, idx) (sa_heap_nth(heap, left_index((idx))))
#define sa_heap_right(heap, idx) (sa_heap_nth(heap, right_index((idx))))

static void
sa_heap_swim(sa_heap *heap, int idx) {
  assert(idx < sa_heap_size(heap));
  
  while(has_parent(heap, idx)) {
    int parent_idx = parent_index(idx);
    sa_heap_value swim_val = sa_heap_nth(heap, idx);
    sa_heap_value parent_val = sa_heap_nth(heap, parent_idx);
    
    if (heap->compare_fn(swim_val, parent_val) == 1) {
      sa_vector_swap(heap->store, idx, parent_idx);
      idx = parent_idx;
    } else {
      break;
    }
  }
}

static void
sa_heap_sink(sa_heap *heap, int idx) {
  assert(idx < sa_heap_size(heap));
	 
  while(has_left(heap, idx) || has_right(heap, idx)) {
    int largest_idx = idx;
    
    if (heap->compare_fn(sa_heap_left(heap, idx),
			 sa_heap_nth(heap, idx)) == 1) {
      largest_idx = left_index(idx);
    }
    if (has_right(heap, idx) &&
	heap->compare_fn(sa_heap_right(heap, idx),
			 sa_heap_nth(heap, largest_idx)) == 1) {
      largest_idx = right_index(idx);
    }

    if (largest_idx != idx) {
      sa_vector_swap(heap->store, idx, largest_idx);
      idx = largest_idx;
    } else {
      break;
    }
  }
}

/* public members */

sa_heap *
sa_heap_new(sa_heap_compare_fn compare_fn) {
  assert(compare_fn != NULL);
  
  sa_heap *heap = malloc(sizeof(sa_heap));
  if (heap == NULL) {
    return NULL;
  }
  
  sa_vector *vect = sa_vector_new(NULL);
  if (vect == NULL) {
    free(heap);
    return NULL;
  }
  
  *heap = (sa_heap){.store = vect, .compare_fn = compare_fn};
  return heap;
}

void
sa_heap_free(sa_heap *heap) {
  assert(heap != NULL);
  sa_vector_free(heap->store);
  free(heap);
}

int
sa_heap_put(sa_heap *heap, sa_heap_value value) {
  assert(heap != NULL && value != NULL);

  if (sa_vector_add(heap->store, (sa_vector_value)value) != 0) {
    return -1;
  }
  
  sa_heap_swim(heap, sa_heap_size(heap) - 1);
  return 0;
}

int
sa_heap_extract(sa_heap *heap, sa_heap_value *value) {
  assert(heap != NULL && sa_heap_size(heap) > 0);

  int last_idx = sa_vector_length(heap->store) - 1;
  sa_vector_swap(heap->store, 0, last_idx);
  if (sa_vector_remove_nth(heap->store, last_idx, value) != 0) {
    return -1;
  }

  if (sa_heap_size(heap) > 0) {
    sa_heap_sink(heap, 0);
  }
  
  return 0;
}
