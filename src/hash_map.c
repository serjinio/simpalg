/**
   Sample of a hash store
**/

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

#include "hash_map.h"


#define HASH_BINS_NO 5


/************************************************************
 Sample hash function
************************************************************/

unsigned long
hash_string(unsigned char *str)
{
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash % HASH_BINS_NO;
}


/************************************************************
 Data structures & their functions
************************************************************/

struct sa_hash_item {
  void *value;
  struct sa_hash_item *next;
};

struct sa_hash_items_list {
  struct sa_hash_item *head;
  int size;
};


struct sa_hash *
sa_hash_new() {
  struct sa_hash *hash = malloc(sizeof(struct sa_hash));
  hash->bins_no = HASH_BINS_NO;

  hash->store_lists = malloc(sizeof(struct sa_hash_items_list) * hash->bins_no);
  for (int i = 0; i < hash->bins_no; i++) {
    struct sa_hash_items_list *plst = hash->store_lists + i;
    plst->head = NULL;
    plst->size = 0;
}
  
return hash;
}

_Bool
sa_hash_contains(struct sa_hash *hash, char *value) {
  int value_hash = hash_string(value);
  assert(value_hash < HASH_BINS_NO);
  struct sa_hash_items_list *lst = hash->store_lists + value_hash;
  struct sa_hash_item *item = lst->head;
  
  while (item != NULL) {
    char *item_value = (char *)item->value;
    if (strcmp(item_value, value) == 0) {
      return true;
    }
    item = item->next;
  }

  return false;
}

int
sa_hash_add(struct sa_hash *hash, char *value) {
  if (sa_hash_contains(hash, value)) {
    return -2;
  }

  // construct new value
  struct sa_hash_item *item = malloc(sizeof(struct sa_hash_item));
  if (item == NULL) {
    return -1;
  }
  item->value = value;
  item->next = NULL;
  
  // put it into hash
  int value_hash = hash_string(value);
  assert(value_hash < HASH_BINS_NO);
  struct sa_hash_items_list *lst = hash->store_lists + value_hash;
  if (lst->head == NULL) {
    lst->head = item;
  } else {
    item->next = lst->head;
    lst->head = item;
  }
  lst->size += 1;
  
  return 0;
}

int
sa_hash_count(struct sa_hash *hash) {
  int count = 0;
  for (int i = 0; i < hash->bins_no; i++) {
    struct sa_hash_items_list *lst = hash->store_lists + i;
    count += lst->size;
  }
  return count;
}

void
sa_hash_items_list_print(struct sa_hash_items_list *lst) {
  struct sa_hash_item *item = lst->head;
  while (item != NULL) {
    printf("  '%s'\n", (char *)item->value);
    item = item->next;
  }
}

void
sa_hash_print(struct sa_hash *hash) {
  printf("printing hash contents. hash bins no.: %d\n", hash->bins_no);
  for (int i = 0; i < hash->bins_no; i++) {
    printf("contents of bin no.: %d\n:", i);
    struct sa_hash_items_list *lst = hash->store_lists + i;
    sa_hash_items_list_print(lst);
  }
}

void
sa_hash_print_bin_lengths(struct sa_hash *hash) {
  for (int i = 0; i < hash->bins_no; i++) {
    struct sa_hash_items_list *lst = hash->store_lists + i;
    printf("bin # %d length: %d\n", i, lst->size);
  }
}

