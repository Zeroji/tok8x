#ifndef TOK8X_PARSE_H
#define TOK8X_PARSE_H

#include <string.h>
#include <uthash.h>
#include "buf.h"
#include "hash.h"

typedef struct parse_context_s parse_context_t;
struct parse_context_s {
	unsigned int line;
	unsigned int col;
	unsigned int offset;
	unsigned int line_offset;
};

tok_t* parse_byte(buf_t *b, parse_context_t *pc, hash_pair_t *hp);
tok_t* parse_str(buf_t *b, parse_context_t *pc, hash_pair_t *hp);

buf_t* parse_buf_byte(buf_t *b, t_list_t list, bool pretty);
buf_t* parse_buf_str(buf_t *b, t_list_t list, bool pretty);

#endif
