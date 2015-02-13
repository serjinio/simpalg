/*
  A hash map container.
 */

#ifndef SA_HASH_MAP_H
#define SA_HASH_MAP_H

/* sa_hash - a sample hash declaration */
struct sa_hash {
  struct sa_hash_items_list *store_lists;
  int bins_no;
};

struct sa_hash *sa_hash_new();
_Bool sa_hash_contains(struct sa_hash *hash, char *value);
int sa_hash_add(struct sa_hash *hash, char *value);
int sa_hash_count(struct sa_hash *hash);

/* debugging stuff */
void sa_hash_print(struct sa_hash *hash);
void sa_hash_print_bin_lengths(struct sa_hash *hash);


#ifdef __cplusplus
}
#endif
  
#endif
