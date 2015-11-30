#include "hash.h"

static int name_sort(tok_t *a, tok_t *b)
{
	int la, lb;

	la = strlen(a->name);
	lb = strlen(b->name);

	if(la > lb)
		return -1;
	if(la < lb)
		return 1;
	return strcmp(a->name, b->name);
}

static hash_t* hash_init_str_sub(t_list_t list)
{
	int i;
	hash_t *h = NULL;

	for(i = 0; i < t_list_lengths[list]; i++)
		HASH_ADD(hhs, h, name, TOKLEN*sizeof(char), (&(t_lists[list][i])) );

	HASH_SRT(hhs, h, name_sort);

	return h;
}

hash_pair_t* hash_init_str(t_list_t list, bool pretty)
{
	hash_pair_t *hp = hash_pair_new();

	EXIT_NULL(hp);
	
	hp->standard = NULL;
	hp->pretty = NULL;
	hp->list = list;

	hp->standard = hash_init_str_sub(list);

	if(pretty) {
		hp->pretty = hash_init_str_sub(list+(LIST_COUNT / 2));
	}

	return hp;
}

static hash_t* hash_init_byte_sub(t_list_t list)
{
	int i;
	hash_t *h = NULL, *tmp;

	for(i = 0; i < t_list_lengths[list]; i++) {
		/* if 2byte */
		if( t_is2byte(t_lists[list][i].b1) ) {
			/* check if the sub-hash exists and,
			 * if not, create and add it */
			HASH_FIND(hht, h, (&(t_lists[list][i].b1)),
					sizeof(uint8_t), tmp);

			if(tmp == NULL) {
				tmp = malloc(sizeof(tok_t));
				tmp->b1 = t_lists[list][i].b1;
				tmp->is_subhash = true;
				tmp->subhash = NULL;
				HASH_ADD(hht, h, b1, sizeof(uint8_t), tmp);
			}

			/* add the 2byte tok to the subhash */
			HASH_ADD(hht, tmp->subhash, b2, sizeof(uint8_t),
					(&(t_lists[list][i])) );
		/* if not 2byte */
		} else {
			HASH_ADD(hht, h, b1, sizeof(uint8_t), (&(t_lists[list][i])) );
		}
	}

	return h;
}

hash_pair_t* hash_init_byte(t_list_t list, bool pretty)
{
	hash_pair_t *hp = hash_pair_new();

	EXIT_NULL(hp);

	hp->standard = NULL;
	hp->pretty = NULL;
	hp->list = list;

	hp->standard = hash_init_byte_sub(list);

	if(pretty) {
		hp->pretty = hash_init_byte_sub(list+(LIST_COUNT / 2));
	}

	return hp;
}

void hash_free(hash_t *h)
{

}

hash_pair_t* hash_pair_new(void)
{
	hash_pair_t *hp = malloc(sizeof(hash_pair_t));

	EXIT_NULL(hp);

	return hp;
}

void hash_pair_free(hash_pair_t *h)
{

}
