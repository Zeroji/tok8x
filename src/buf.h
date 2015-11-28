#ifndef BUF_H
#define BUF_H

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
void buf_push_char(char c, buf_t *b);
void buf_push_str(char *s, buf_t *b);

/* read in a buffer from a file or stream */
void buf_read(FILE *f, buf_t *b);

buf_t* buf_new(void);
void buf_free(buf_t *b);

#endif
