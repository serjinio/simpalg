
/*
  A doubly linked list container.
*/

#ifndef SA_LIST_H
#define SA_LIST_H


#include <stdbool.h>


typedef void *sa_list_val;

typedef struct _sa_list_node {
  sa_list_val value;
  struct _sa_list_node *next;
  struct _sa_list_node *prev;
} sa_list_node;

typedef _Bool (*sa_list_equals_fn)(sa_list_val val1, sa_list_val val2);

typedef struct _sa_list {
  sa_list_node *head;
  sa_list_node *tail;
  unsigned long length;
  sa_list_equals_fn equals_fn;
} sa_list;


sa_list *sa_list_new(sa_list_equals_fn equals_fn);
void sa_list_free(sa_list *lst);

int sa_list_prepend(sa_list *lst, sa_list_val value);
int sa_list_append(sa_list *lst, sa_list_val value);

sa_list_node *sa_list_find(sa_list *lst, sa_list_val val);
sa_list_val sa_list_remove(sa_list *lst, sa_list_node *node);

#define sa_list_head(lst) ((lst)->head)
#define sa_list_tail(lst) ((lst)->tail)

#define sa_list_next(node) ((node)->next)
#define sa_list_prev(node) ((node)->prev)
#define sa_list_value(node) ((node)->value)

#endif
