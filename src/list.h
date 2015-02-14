
/*
  A doubly linked list container.
*/

#ifndef SA_LIST_H
#define SA_LIST_H


#include <stdbool.h>

#include "list.h"


typedef void *sa_list_value;

typedef struct sa_list_node {
  sa_list_value value;
  struct sa_list_node *next;
  struct sa_list_node *prev;
} sa_list_node;

typedef _Bool (*sa_list_equals_fn)(sa_list_value val1, sa_list_value val2);

typedef struct sa_list {
  sa_list_node *head;
  sa_list_node *tail;
  unsigned long length;
  sa_list_equals_fn equals_fn;
} sa_list;


sa_list *sa_list_new(sa_list_equals_fn equals_fn);
void sa_list_free(sa_list *lst);

int sa_list_prepend(sa_list *lst, sa_list_value value);
int sa_list_append(sa_list *lst, sa_list_value value);

sa_list_node *sa_list_find(sa_list *lst, sa_list_value val);
sa_list_value sa_list_remove(sa_list *lst, sa_list_node *node);

#define sa_list_head(lst) ((lst)->head)
#define sa_list_tail(lst) ((lst)->tail)

#define sa_list_next(node) ((node)->next)
#define sa_list_prev(node) ((node)->prev)
#define sa_list_value(node) ((node)->value)

#define sa_list_length(lst) ((lst)->length)
#define sa_list_contains(lst, value) (sa_list_find((lst), (value)) != NULL)

/* debugging functions */
void sa_list_print(sa_list *lst);

#endif
