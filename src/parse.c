#include "parse.h"

static tok_t* parse_byte_sub(buf_t *b, parse_context_t *pc, hash_t *h)
{
	tok_t *result1, *result2;

	HASH_FIND(hhb, h, (&(((uint8_t*)(b->content))[pc->offset])),
			sizeof(uint8_t), result1);

	/* nothing found */
	if(result1 == NULL)
		return NULL;

	/* 1byte found */
	if( !(result1->is_subhash) ) {
		pc->offset++;
		return result1;
	}

	/* check if 2byte will overflow */
	if( (pc->offset + 1) == b->content_size)
		return NULL;

	HASH_FIND(hhb, result1->subhash, (&(((uint8_t*)(b->content))[pc->offset+1])),
			sizeof(uint8_t), result2);

	/* 2byte not found */
	if(result2 == NULL)
		return NULL;

	/* 2byte found */
	pc->offset += 2;
	return result2;
}

tok_t* parse_byte(buf_t *b, parse_context_t *pc, hash_pair_t *hp)
{
	tok_t *result;

	if(hp->pretty) {
		result = parse_byte_sub(b, pc, hp->pretty);
		if(result != NULL)
			return result;
	}

	result = parse_byte_sub(b, pc, hp->standard);
	return result;
}

static tok_t* parse_str_sub(buf_t *b, parse_context_t *pc, hash_t *h, bool single)
{
	tok_t *current, *tmp;
	int current_len;
	HASH_ITER(hhs, h, current, tmp) {
		current_len = wcslen(current->name);
		if(pc->offset + current_len <= b->content_size) {
			if( !wcsncmp( (b->content)+(pc->offset), current->name,
						current_len) ) {
				if(!single || current_len == 1) {
					pc->col++;
					if( !wcsncmp(current->name, L"\n", 1) ){
						pc->col = 0;
						pc->line++;
						pc->line_start = pc->offset + 1;
					}
					pc->offset += wcslen(current->name);
					return current;
				}
			}
		}
	}

	return NULL;
}

tok_t* parse_str(buf_t *b, parse_context_t *pc, hash_pair_t *hp, bool single)
{
	tok_t *result;

	result = parse_str_sub(b, pc, hp->standard, single);

	return result;
}

/* error printing */
static void parse_buf_byte_err(buf_t *bin, parse_context_t *pc, char *fname)
{
	unsigned int i, j, width = 80;

#ifdef TIOCGSIZE
	struct ttysize ts;
	ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
	width = ts.ts_cols;
#elif defined(TIOCGWINSZ)
	struct winsize ts;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
	width = ts.ws_col;
#endif /* TIOCGSIZE */

	fprintf(stderr, "\e[1m%s:0x%2X: \e[1;31merr:\e[0m ",
			fname, (pc->offset) + HEADER_SIZE);

	fprintf(stderr, "unrecognised token\n");

	/* too thin for pretty messages or unknown device, so width
	 * can't be determined */
	if(width <= 19)
		return;

	fprintf(stderr, "    ");

	j = 4;
	i = pc->offset;
	if(i) i--;
	if(i) i--;

	for(; i < pc->offset + 3 && i < bin->content_size; i++) {
		if(i == pc->offset)
			fprintf(stderr, "\e[1m%02X\e[0m ", ((uint8_t*)(bin->content))[i]);
		else
			fprintf(stderr, "%02X ", ((uint8_t*)(bin->content))[i]);
		if(i < pc->offset)
			j += 3;
	}

	fprintf(stderr, "\n");

	for(i = 0; i < j; i++)
		fprintf(stderr, " ");

	fprintf(stderr, "\e[1;32m^\e[0m\n");
}

buf_t* parse_buf_byte(buf_t *bin, buf_t *bout, t_list_t list, char *fname, bool pretty, bool safe)
{
	hash_pair_t *hp = hash_init_byte(list, pretty);
	parse_context_t pc = { 0 };
	tok_t *current;

	EXIT_NULL(hp);

	for(;;) {
		if(pc.offset >= bin->content_size)
			break;

		current = parse_byte(bin, &pc, hp);

		if(current == NULL) {
			parse_buf_byte_err(bin, &pc, fname);
			buf_free(bout);
			bout = NULL;
			break;
		}

		if(safe && current->make_safe)
			buf_push_wstr(bout, L"\\");

		buf_push_wstr(bout, current->name);
	}

	hash_pair_free(hp);

	return bout;
}

static void parse_buf_str_err(buf_t *bin, parse_context_t *pc, char *fname)
{
//	unsigned int i, j, line_length, offleft, offright, width = 80;
//	wchar_t *line;
//
//#ifdef TIOCGSIZE
//	struct ttysize ts;
//	ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
//	width = ts.ts_cols;
//#elif defined(TIOCGWINSZ)
//	struct winsize ts;
//	ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
//	width = ts.ws_col;
//#endif /* TIOCGSIZE */
//
//	/* calculate length of line */
//	i = pc->line_start;
//	while(i < bin->content_size && ((wchar_t*)(bin->content))[i] != L'\n') {
//		i++;
//	}
//	line_length = i - pc->line_start;
//
//	/* if line is wider than the screen */
//	if(line_length > width) {
//		i = 0;
//		/* calculate the best starting character for the line */
//		while(pc->offset - pc->line_start > (width/2)
//				&& i < ((pc->offset - pc->line_start) - (width/2))
//				&& i + width < line_length)
//			i++;
//
//		/* should the left side be offset for ... ? */
//		offleft = (i > 0 ? 3 : 0);
//
//		/* should the right side be offset for ... ? */
//		offright = (i + width < line_length ? 3 : 0);
//
//		if(offleft != 0)
//			memcpy(line, "...", offleft * sizeof(char));
//
//		memcpy(line + offleft, bin->content + pc->line_start + i + offleft,
//				(width - offleft - offright) * sizeof(char));
//
//		if(offright != 0)
//			memcpy(line + width - offright, "...", offright * sizeof(char));
//
//		j = pc->offset - pc->line_start - i;
//	} else {
//		memcpy(line, bin->content + pc->line_start, line_length * sizeof(char));
//		j = pc->offset - pc->line_start;
//	}
//
//	fprintf(stderr, "\e[1m%s:%d:%d: \e[1;31merr:\e[0m ",
//			fname, pc->line, pc->col);
//
//	fprintf(stderr, "unrecognised token\n");
//
//	fprintf(stderr, "%s\n", line);
//
//	for(i = 0; i < j; i++)
//		fprintf(stderr, " ");
//
//	fprintf(stderr, "\e[1;32m^\e[0m\n");
//
//	free(line);
}

buf_t* parse_buf_str(buf_t *bin, buf_t *bout, t_list_t list, char *fname)
{
	hash_pair_t *hp = hash_init_str(list);
	parse_context_t pc = { 0 };
	tok_t *current;

	EXIT_NULL(hp);

	for(;;) {
		if(pc.offset >= bin->content_size)
			break;

		/* check for backslash, which indicates the next
		 * char should be considered literally */
		if(((wchar_t*)(bin->content))[pc.offset] == L'\\') {
			/* if one step forward is out of bounds */
			if(pc.offset + 1 == bin->content_size) {
				printf("%s\n", "call parse_buf_str_err");
				//parse_buf_str_err(bin, &pc, fname);
				buf_free(bout);
				hash_pair_free(hp);
				return NULL;
			}

			pc.offset++;
			current = parse_str(bin, &pc, hp, true);

			goto end;
		}

		current = parse_str(bin, &pc, hp, false);

end:
		if(current == NULL) {
			printf("%s\n", "call parse_buf_str_err");
			//parse_buf_str_err(bin, &pc, fname);
			buf_free(bout);
			hash_pair_free(hp);
			return NULL;
		}

		buf_push_byte(bout, current->b1);
		if(current->b2 != NONE)
			buf_push_byte(bout, current->b2);
	}

	hash_pair_free(hp);

	return bout;
}
