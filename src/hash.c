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
	hp->is_tok = false;

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
	hp->is_tok = true;

	hp->standard = hash_init_byte_sub(list);

	if(pretty) {
		hp->pretty = hash_init_byte_sub(list+(LIST_COUNT / 2));
	}

	return hp;
}

static void hash_free(hash_t *h, bool is_tok)
{
	tok_t *current1, *current2, *tmp1, *tmp2;

	EXIT_NULL(h);

	/* have to split these up because hht/hhs are literal names */
	if(is_tok) {
		HASH_ITER(hht, h, current1, tmp1) {
			if(current1->is_subhash) {
				puts("2 - entering subhash");
				HASH_ITER(hht, current1->subhash, current2, tmp2) {
					printf("2 - deleting %s\n", current2->name);
					HASH_DELETE(hht, current1->subhash, current2);
				}
				HASH_DELETE(hht, h, current1);
				free(current1);
			} else {
				printf("1 - deleting %s\n", current1->name);
				HASH_DELETE(hht, h, current1);
			}
		}
	} else {
		HASH_ITER(hhs, h, current1, tmp1) {
			HASH_DELETE(hhs, h, current1);
		}
	}
}

hash_pair_t* hash_pair_new(void)
{
	hash_pair_t *hp = malloc(sizeof(hash_pair_t));

	EXIT_NULL(hp);

	return hp;
}

void hash_pair_free(hash_pair_t *hp)
{
	EXIT_NULL(hp);
	EXIT_NULL(hp->standard);

	hash_free(hp->standard, hp->is_tok);

	if(hp->pretty != NULL)
		hash_free(hp->pretty, hp->is_tok);

	free(hp);
}
