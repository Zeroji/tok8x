#ifndef TOK8X_BUF_H
#define TOK8X_BUF_H

#include <stdio.h>
#include <string.h>
#include "err.h"

/* contains input file content in mem */
typedef struct buf_s buf_t;
struct buf_s {
	char *buf_content;
	unsigned buf_size;
	unsigned buf_content_size;
};

/* append a character to the end of a buffer */
void buf_push_char(buf_t *b, char c);
void buf_push_str(buf_t *b, char *s);

/* read in a buffer from a file or stream */
void buf_read(buf_t *b, FILE *f,);

buf_t* buf_new(void);
void buf_free(buf_t *b);

#endif
