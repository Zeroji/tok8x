#include <stdio.h>
#include <locale.h>
#include "opt.h"
#include "buf.h"
#include "parse.h"
#include "header.h"

#define MAIN_ERR(en, format, ...) \
	do { \
		fprintf(stderr, "\e[1mtok8x:\e[0m "); \
		fprintf(stderr, "\e[1;31merror:\e[0m " format "\n", ##__VA_ARGS__); \
		exit(en); \
	} while(0)

#define MAIN_ERR_CONV(en, format, ...) \
	do { \
		if(b != NULL) \
			buf_free(b); \
		if(bswap != NULL) \
			buf_free(bswap); \
		opt_free(o); \
		MAIN_ERR(en, format, ##__VA_ARGS__); \
	} while(0)


static void sub_info(opt_t *o)
{
	if(o->extra_args == NULL)
		MAIN_ERR(EINVAL, "no tokens provided");

	/* TODO: implement sub_info */
}

static void sub_convert(opt_t *o)
{
	buf_t *b = NULL, *bswap = NULL;
	FILE *f;
	bool is_8xp;
	int i;

	/* if files provided as arguments */
	if(o->extra_args != NULL) {
		bswap = buf_new();
		i = 0;
		EXIT_NULL(o->extra_args[0]);
		while(o->extra_args[i] != NULL) {
			/* read file */
			f = fopen(o->extra_args[i], "r");
			if(f == NULL) {
				MAIN_ERR_CONV(errno, "could not open file \"%s\" for reading",
						o->extra_args[i]);
			}
			b = buf_read(f);
			fclose(f);
			if(i == 0)
				is_8xp = b->is_8xp;
			else {
				if(is_8xp != b->is_8xp)
					MAIN_ERR_CONV(EPERM, "inconsistent input file types");
			}

			/* convert file and append to bswap */
			if(is_8xp) {
				/* TODO: expand */
				bswap = parse_buf_byte(b, bswap, o->list, o->extra_args[i],
						o->pretty, o->safe);
			} else {
				bswap = parse_buf_str(b, bswap, o->list, o->extra_args[i]);
			}
			buf_free(b);
			b = NULL;

			/* error converting. */
			if(bswap == NULL) {
				opt_free(o);
				exit(EINVAL);
			}

			i++;
		}
	} else {
		/* read from stdin */
		b = buf_read(stdin);
		is_8xp = b->is_8xp;

		/* convert stdin */
		if(is_8xp) {
			bswap = parse_buf_byte(b, bswap, o->list, "stdin",
					o->pretty, o->safe);
		} else {
			bswap = parse_buf_str(b, bswap, o->list, "stdin");
		}
		buf_free(b);
		b = NULL;
		
		/* error converting. */
		if(bswap == NULL) {
			opt_free(o);
			exit(EINVAL);
		}
	} /* end reading */

	b = bswap;
	bswap = NULL;
	
	/* pack, if necessary */
	if(!is_8xp) {
		if(o->name != NULL)
			bswap = header_pack_buf(b, o->name, o->archived);
		else
			bswap = header_pack_buf(b, "A", o->archived);

		buf_free(b);
		b = bswap;
		bswap = NULL;

		/* TODO: compact */
	}

	/* write to file */
	if(o->output != NULL) {
		f = fopen(o->output, "w");
		if(f == NULL) {
			MAIN_ERR_CONV(errno, "could not open file \"%s\" for writing",
					o->output);
		}
		buf_write(b, f);
		fclose(f);
		buf_free(b);
		return;
	} else {
		buf_write(b, stdout);
		buf_free(b);
		return;
	}
}

int main(int argc, const char *argv[])
{
	opt_t *o;

	setlocale(LC_ALL, "");

	o = opt_read(argc, argv);

	if(o->info)
		sub_info(o);
	else
		sub_convert(o);

	opt_free(o);

	return 0;
}
