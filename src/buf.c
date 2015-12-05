#include "buf.h"

void buf_push_byte(buf_t *b, uint8_t y)
{
	EXIT_NULL(b);

	/* increase size if necessary */
	if(b->content_size+1 == b->size) {
		b->content = realloc(b->content, b->size*2*sizeof(uint8_t));
		EXIT_NULL(b->content);
		b->size *= 2;
	}

	b->content[b->content_size] = y;
	b->content_size++;
}

void buf_push_nbyte(buf_t *b, uint8_t *y, int n)
{
	int i;

	EXIT_NULL(b);

	for(i = 0; i < n; i++) {
		buf_push_byte(b, y[i]);
	}
}

void buf_push_str(buf_t *b, char *s)
{
	int i;

	EXIT_NULL(b);
	EXIT_NULL(s);

	for(i = 0; i < strlen(s); i++) {
		buf_push_byte(b, (uint8_t)(s[i]) );
	}
}

buf_t* buf_read(FILE *f)
{
	char c;
	buf_t *b = buf_new();

	EXIT_NULL(b);

	for(;;) {
		c = getc(f);
		EXIT_FERROR(f);

		if(c == EOF)
			break;

		buf_push_byte(b, (uint8_t)c);
	}

	return b;
}

void buf_write(buf_t *b, FILE *f)
{
	EXIT_NULL(f);
	EXIT_NULL(b);

	fwrite(b->content, sizeof(uint8_t),
			b->content_size, f);
}

buf_t* buf_new(void)
{
	buf_t *b = malloc(sizeof(buf_t));
	EXIT_NULL(b);

	b->content = malloc(32*sizeof(uint8_t));
	EXIT_NULL(b->content);
	b->size = 32;
	b->content_size = 0;

	return b;
}

void buf_free(buf_t *b)
{
	free(b->content);
	free(b);
}
