#include "buf.h"

void buf_push_byte(buf_t *b, uint8_t y)
{
	EXIT_NULL(b);
	EXIT_NEQUAL(b->is_8xp, true);

	/* increase size if necessary */
	if(b->content_size+1 >= b->size) {
		b->content = realloc(b->content, b->size*2*sizeof(uint8_t));
		EXIT_NULL(b->content);
		b->size *= 2;
	}

	((uint8_t*)(b->content))[b->content_size] = y;
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

void buf_push_wchar(buf_t *b, wchar_t wc)
{
	EXIT_NULL(b);
	EXIT_NEQUAL(b->is_8xp, false);

	/* increase size if necessary */
	if(b->content_size+2 >= b->size) {
		b->content = realloc(b->content, b->size * 2 * sizeof(wchar_t));
		EXIT_NULL(b->content);
		b->size *= 2;
	}

	((wchar_t*)(b->content))[b->content_size] = wc;
	((wchar_t*)(b->content))[b->content_size+1] = L'\0';
	b->content_size++;
}

void buf_push_nwchar(buf_t *b, wchar_t* ws, int n)
{
	int i;

	EXIT_NULL(b);
	EXIT_NULL(ws);

	for(i = 0; i < n; i++)
		buf_push_wchar(b, ws[i]);
}

void buf_push_wstr(buf_t *b, wchar_t *ws)
{
	EXIT_NULL(b);
	EXIT_NULL(ws);

	buf_push_nwchar(b, ws, wcslen(ws));
}

buf_t* buf_read(FILE *f)
{
	const int sigsize = 8;
	wint_t wc;
	uint8_t y;
	int ret;
	buf_t *b;
	wchar_t wcswap[sigsize];
	uint8_t yswap[2];
	int i;

	/* read sig bytes or first sigsize chars */
	for(i = 0; i < sigsize; i++) {
		wc = getwc(f);
		EXIT_FERROR(f);
		if(wc == WEOF) {
			b = buf_new(false);
			EXIT_NULL(b);
			buf_push_nwchar(b, wcswap, i);
			return b;
		}
		wcswap[i] = (wchar_t)wc;
	}

	/* check for sig to determine if 8xp */
	if( !wcsncmp(wcswap, L"**TI83F*", sigsize) ) {
		b = buf_new(true);
	} else {
		b = buf_new(false);
	}

	/* not 8xp, so just read the rest */
	if( !b->is_8xp) {
		buf_push_nwchar(b, wcswap, sigsize);
		while( (wc = getwc(f)) != WEOF) {
			buf_push_wchar(b, (wchar_t)wc);
		}

		return b;
	}

	/* discard remainder of header */
	for(i = sigsize; i < HEADER_SIZE; i++) {
		ret = fread(&y, sizeof(uint8_t), 1, f);
		EXIT_FERROR(f);
		/* EOF this early means malformed 8xp */
		if(ret <= 0) {
			printf("\e[1mtok8x:\e[0m"
					"\e[1;31merror:\e[0m %s\n",
					"malformed .8xp input"
					);
			exit(EIO);
		}
	}


	/* if 8xp, there will be, at fewest, 2 more bytes to be read (the checksum) */
	ret = fread(&y, sizeof(uint8_t), 1, f);
	EXIT_FERROR(f);
	if(ret <= 0) {
		printf("\e[1mtok8x:\e[0m"
				"\e[1;31merror:\e[0m %s\n",
				"malformed .8xp input"
				);
		exit(EIO);
	}
	yswap[0] = y;

	ret = fread(&y, sizeof(uint8_t), 1, f);
	EXIT_FERROR(f);
	if(ret <= 0) {
		printf("\e[1mtok8x:\e[0m"
				"\e[1;31merror:\e[0m %s\n",
				"malformed .8xp input"
				);
		exit(EIO);
	}
	yswap[1] = y;

	for(;;) {
		ret = fread(&y, sizeof(uint8_t), 1, f);
		EXIT_FERROR(f);

		if(ret <= 0)
			break;

		buf_push_byte(b, yswap[0]);
		yswap[0] = yswap[1];
		yswap[1] = y;
	}

	return b;
}

void buf_write(buf_t *b, FILE *f)
{
	EXIT_NULL(f);
	EXIT_NULL(b);

	if(b->is_8xp)
		fwrite(b->content, sizeof(uint8_t), b->content_size, f);
	else
		fputws((wchar_t*)(b->content), f);
}

buf_t* buf_new(bool is_8xp)
{
	buf_t *b = malloc(sizeof(buf_t));
	EXIT_NULL(b);

	if(is_8xp)
		b->content = malloc(32*sizeof(uint8_t));
	else
		b->content = malloc(32*sizeof(wchar_t));

	EXIT_NULL(b->content);

	b->size = 32;
	b->content_size = 0;
	b->is_8xp = is_8xp;

	return b;
}

void buf_free(buf_t *b)
{
	free(b->content);
	free(b);
}
