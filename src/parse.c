#include "parse.h"

static tok_t* parse_byte_sub(buf_t *b, parse_context_t *pc, hash_t *h)
{
	tok_t *result1, *result2;

	HASH_FIND(hhb, h, (&(b->content[pc->offset])), sizeof(uint8_t), result1);

	/* nothing found */
	if(result1 == NULL)
		return NULL;

	/* 1byte found */
	if( !(result1->is_subhash) ) {
		pc->offset++;
		return result1;
	}

	HASH_FIND(hhb, result1->subhash, (&(b->content[pc->offset+1])), sizeof(uint8_t), result2);

	/* 2byte not found */
	if(result2 == NULL)
		return NULL;


	/* 2byte found */
	pc->offset += 2;
	return result2;
}

tok_t* parse_byte(buf_t *b, parse_context_t *pc, hash_pair_t *hp)
{
	tok_t *result;

	if(hp->pretty) {
		result = parse_byte_sub(b, pc, hp->pretty);
		if(result != NULL)
			return result;
	}

	result = parse_byte_sub(b, pc, hp->standard);
	return result;
}

static tok_t* parse_str_sub(buf_t *b, parse_context_t *pc, hash_t *h)
{
	tok_t *current, *tmp;
	HASH_ITER(hhs, h, current, tmp) {
		if( !memcmp( (b->content)+(pc->offset), (uint8_t*)current->name,
					strlen(current->name) ) ) {
			pc->offset += strlen(current->name);
			return current;
		}
	}

	return NULL;
}

tok_t* parse_str(buf_t *b, parse_context_t *pc, hash_pair_t *hp)
{
	tok_t *result, *presult = NULL;

	if(hp->pretty) {
		presult = parse_str_sub(b, pc, hp->pretty);
	}

	result = parse_str_sub(b, pc, hp->standard);

	if(presult != NULL) {
		if(result == NULL)
			return presult;

		if(strlen(presult->name) >= strlen(result->name))
			return presult;
	}

	return result;
}

buf_t* parse_buf_byte(buf_t *bin, t_list_t list, bool pretty)
{
	buf_t *bout = buf_new();
	hash_pair_t *hp = hash_init_byte(list, pretty);
	parse_context_t pc = { 0 };
	tok_t *current;

	EXIT_NULL(bout);
	EXIT_NULL(hp);

	while( (current = parse_byte(bin, &pc, hp)) != NULL ) {
		buf_push_str(bout, current->name);
	}

	return bout;
}

buf_t* parse_buf_str(buf_t *bin, t_list_t list, bool pretty)
{
	buf_t *bout = buf_new();
	hash_pair_t *hp = hash_init_str(list, pretty);
	parse_context_t pc = { 0 };
	tok_t *current;

	EXIT_NULL(bout);
	EXIT_NULL(hp);

	while( (current = parse_str(bin, &pc, hp)) != NULL ) {
		buf_push_byte(bout, current->b1);

		if(current->b2 != NONE)
			buf_push_byte(bout, current->b2);
	}

	return bout;
}
