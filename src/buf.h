#ifndef TOK8X_BUF_H
#define TOK8X_BUF_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "err.h"

#define HEADER_SIZE 0x4A

/* contains input file content in mem */
typedef struct buf_s buf_t;
struct buf_s {
	/* can point either to uint8_t or wchar_t */
	void *content;
	unsigned size;
	unsigned content_size;
	bool is_8xp;
};

/* append a byte to the end of a buffer */
void buf_push_byte(buf_t *b, uint8_t y);
void buf_push_nbyte(buf_t *b, uint8_t *y, int n);
void buf_push_wchar(buf_t *b, wchar_t wc);
void buf_push_nwchar(buf_t *b, wchar_t* ws, int n);
void buf_push_wstr(buf_t *b, wchar_t *ws);

buf_t* buf_read(FILE *f);
void buf_write(buf_t *b, FILE *f);

buf_t* buf_new(bool is_8xp);
void buf_free(buf_t *b);

#endif
