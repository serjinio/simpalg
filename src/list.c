
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "list.h"


/* Private helper functions */

static sa_list_node *
sa_list_node_new() {
  sa_list_node *node = malloc(sizeof(sa_list_node));
  if (node == NULL) {
    return NULL;
  }
  
  *node = (sa_list_node){.value = NULL, .prev = NULL, .next = NULL};
  return node;
}

static void
sa_list_node_free(sa_list_node *node) {
  assert(node != NULL);
  free(node);
}

static _Bool
is_node_in_list(sa_list_node *node, sa_list *lst) {
  assert(lst != NULL && node != NULL);
  
  sa_list_node *lst_node = lst->head;
  while (lst_node != NULL) {
    if (lst_node == node) {
      return true;
    }
    lst_node = lst_node->next;
  }

  return false;
}

/* List interface implemnetation */

sa_list *
sa_list_new(sa_list_equals_fn equals_fn) {
  assert(equals_fn != NULL);
  
  sa_list *lst = malloc(sizeof(sa_list));
  *lst = (sa_list){.head = NULL, .tail = NULL, .length = 0,
		    .equals_fn = equals_fn};
  return lst;
}

void
sa_list_free(sa_list *lst) {
  assert(lst != NULL);
  free(lst);
}

int
sa_list_prepend(sa_list *lst, sa_list_value value) {
  assert(lst != NULL && value != NULL);
  sa_list_node *node = sa_list_node_new();
  if (node == NULL) {
    return -1;
  }
  node->value = value;
  
  if (lst->head != NULL) {
    sa_list_node *old_head = lst->head;
    node->next = old_head;
    lst->head = node;
    old_head->prev = node;
  } else {
    assert(lst->length == 0);
    lst->head = node;
    lst->tail = node;
  }
  lst->length += 1;
  
  return 0;
}

int
sa_list_append(sa_list *lst, sa_list_value value) {
  assert(lst != NULL && value != NULL);
  sa_list_node *node = sa_list_node_new();
  if (node == NULL) {
    return -1;
  }
  node->value = value;
  
  if (lst->tail != NULL) {
    sa_list_node *old_tail = lst->tail;
    node->prev = old_tail;
    lst->tail = node;
    old_tail->next = node;
  } else {
    assert(lst->length == 0);
    lst->head = node;
    lst->tail = node;
  }
  lst->length += 1;

  return 0;
}
 
sa_list_node *
sa_list_find(sa_list *lst, sa_list_value val) {
  assert(lst != NULL && lst->equals_fn != NULL);
  
  sa_list_node *lst_node = sa_list_head(lst);
  while (lst_node != NULL) {
    if (lst->equals_fn(sa_list_value(lst_node), val) == true) {
      return lst_node;
    }
    lst_node = sa_list_next(lst_node);
  }

  return NULL;
}

sa_list_value
sa_list_remove(sa_list *lst, sa_list_node *node) {
  if (!is_node_in_list(node, lst)) {
    return NULL;
  }

  if (lst->head == node && lst->tail == node) {
    assert(lst->length == 1);
    lst->head = NULL;
    lst->tail = NULL;
  } else if (lst->head == node) {
    lst->head = lst->head->next;
    lst->head->prev = NULL;
  } else if (lst->tail == node) {
    lst->tail = lst->tail->prev;
    lst->tail->next = NULL;
  }
  lst->length -= 1;
  
  sa_list_value node_value = node->value;
  sa_list_node_free(node);
  return node_value;
}


/* debugging functions */

void
sa_list_print(sa_list *lst) {
  assert(lst != NULL);
  sa_list_node *lst_node = sa_list_head(lst);
  while (lst_node != NULL) {
    if (sa_list_value(lst_node) == NULL) {
      printf("NULL\n");
      continue;
    }
    
    printf("%s\n", lst_node->value);
  }
}
