#ifndef TOK8X_PARSE_H
#define TOK8X_PARSE_H

#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <uthash.h>
#include "buf.h"
#include "hash.h"
#include "header.h"

typedef struct parse_context_s parse_context_t;
struct parse_context_s {
	unsigned int line;
	unsigned int col;
	unsigned int line_start;
	unsigned int offset;
};

tok_t* parse_byte(buf_t *b, parse_context_t *pc, hash_pair_t *hp);
/* single indicates the next single character should be treated as a token */
tok_t* parse_str(buf_t *b, parse_context_t *pc, hash_pair_t *hp, bool single);

/* safe mode includes \\ for potentially ambiguous characters so programs can
 * be converted back and forth without worrying about what were originally
 * literal strings being interpreted as tokens */
buf_t* parse_buf_byte(buf_t *b, t_list_t list, char *fname, bool pretty, bool safe);
buf_t* parse_buf_str (buf_t *b, t_list_t list, char *fname);

#endif
