#ifndef	TOK8X_HASH_H
#define	TOK8X_HASH_H

#include <uthash.h>
#include "tok.h"
#include "err.h"

typedef tok_t hash_t;

typedef struct hash_pair_s hash_pair_t;
struct hash_pair_s {
	/* this indicates there will be dynamically
	 * allocated tokens to hold sub-hashes */
	bool is_tok;
	t_list_t list;
	hash_t *standard;
	hash_t *pretty;
};

hash_pair_t* hash_init_str(t_list_t list, bool pretty);
hash_pair_t* hash_init_byte(t_list_t list, bool pretty);

void hash_free(hash_t *h);

hash_pair_t* hash_pair_new(void);
void hash_pair_free(hash_pair_t *h);

#endif
