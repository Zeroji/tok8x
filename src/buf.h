#ifndef TOK8X_BUF_H
#define TOK8X_BUF_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "err.h"

#define HEADER_SIZE 0x4A

/* contains input file content in mem */
typedef struct buf_s buf_t;
struct buf_s {
	uint8_t *content;
	unsigned size;
	unsigned content_size;
	bool is_8xp;
};

/* append a byte to the end of a buffer */
void buf_push_byte(buf_t *b, uint8_t y);
void buf_push_nbyte(buf_t *b, uint8_t *y, int n);
void buf_push_str(buf_t *b, char *s);

buf_t* buf_read(FILE *f);
void buf_write(buf_t *b, FILE *f);

buf_t* buf_new(void);
void buf_free(buf_t *b);

#endif
