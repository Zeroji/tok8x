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

void buf_read(buf_t *b, FILE *f)
{
	char c;
	uint8_t swap[HEADER_SIZE];
	int i;

	for(i = 0; i < HEADER_SIZE; i++) {
		c = getc(f);
		EXIT_FERROR(f);
		/* EOF this early means definitely not 8xp */
		if(c == EOF) {
			buf_push_nbyte(b, swap, i+1);
			b->is_8xp = false;
		}
		swap[i] = (uint8_t)c;
	}

	/* if 8xp, discard header */
	if( !memcmp(swap, (uint8_t*)"**TI83F*", 8*sizeof(uint8_t)) ) {
		b->is_8xp = true;
	} else {
		buf_push_nbyte(b, swap, HEADER_SIZE);
		b->is_8xp = false;
	}

	/* if 8xp, there will be, at fewest, 2 more bytes to be read (the checksum) */
	if(b->is_8xp) {
		c = getc(f);
		EXIT_FERROR(f);
		if(c == EOF) {
			printf("\e[1mtok8x:\e[0m"
					"\e[1;31merror:\e[0m %s\n",
					"malformed .8xp input"
					);
			exit(EIO);
		}

		swap[0] = (uint8_t)c;
		c = getc(f);
		EXIT_FERROR(f);
		if(c == EOF) {
			printf("\e[1mtok8x:\e[0m"
					"\e[1;31merror:\e[0m %s\n",
					"malformed .8xp input"
					);
			exit(EIO);
		}
		swap[1] = (uint8_t)c;

		for(;;) {
			c = getc(f);
			EXIT_FERROR(f);

			if(c == EOF)
				break;

			buf_push_byte(b, swap[0]);
			swap[0] = swap[1];
			swap[1] = (uint8_t)c;
		}
		
		return;
	}

	/* if not 8xp, just read what's left */
	for(;;) {
		c = getc(f);
		EXIT_FERROR(f);

		if(c == EOF)
			break;

		buf_push_byte(b, (uint8_t)c);
	}
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
