#ifndef TOK8X_BUF_H
#define TOK8X_BUF_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "err.h"

/* contains input file content in mem */
typedef struct buf_s buf_t;
struct buf_s {
	uint8_t *buf_content;
	unsigned buf_size;
	unsigned buf_content_size;
};

/* append a byte to the end of a buffer */
void buf_push_byte(buf_t *b, uint8_t y);
void buf_push_nbyte(buf_t *b, uint8_t *y, int n);

/* read in a buffer from a file or stream */
void buf_read(buf_t *b, FILE *f);

buf_t* buf_new(void);
void buf_free(buf_t *b);

#endif
