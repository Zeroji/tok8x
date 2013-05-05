#include "tok8x.h"

int main(int argc, char **argv) {
	uint32_t i, j, k, bad_arg;
	char swapchar;
	header h;
	struct header *h_point=&h;
	FILE *i_file;
	buffer i_buffer;
	i_buffer.dat=NULL;
	char *i_swapbuffer;
	FILE *o_file;
	node *o_buffer;
	node *trav;
	int operation_type_flag=1; /* set to "tokenising" as default. if we later find that the
								* input was an 8x file, we'll set it to "detokenising" */
	
	/* input argument vars */
	char *a_ifilename=NULL;
	char *a_ofilename=NULL;
	node *a_token=NULL;
	int a_strip_cruft=0; /* axe one line comments start with \n., grammer with // 
								axe multi-line comments use ... (be sure to ignore
								conditional comments (...If, ...!If, ...Else ) */
	int a_ignore_errors=0;
	int a_pretty=0;
	uint8_t a_archived=0x00;
	t_set a_t_set=BASIC;
	char a_internal_name[9]={0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	char *help_message="\noptions:\n -h\n   show this help dialogue\n\n -t <axe|basic|grammer>\n   define token set to be used\n\n -i <filename>\n   define file to be read from (defaults to stdin)\n\n -o <filename>\n   define file to be written (defaults to stdout)\n\n -n <name>\n   define on-calc name (warning: does not check name validity)\n\n -a\n   generate archived program\n\n -p\n   use \"pretty\" output (use unicode approximations that\n   more closely resemble the true appearances)\n\n -s\n   strip excess data (comments, spaces, empty lines, etc)\n\n -f\n   force (skip over any unmatched tokens rather than generating an error)\n\n";
	char *usage_message="usage:\n %s -i <filename> [options]\n %s <token name>\n";
	
/* ----------------------[ INPUT PARSING ]---------------------- */
	
	i=1;
	while(argv[i]){
		bad_arg=1;
		if(strncmp(argv[i], "-", 1)) {
			if( !(strlen(argv[i]) > 16) ) {
				if(a_token) {
					trav->next=malloc(sizeof(node));
					if(trav->next) {
						trav=trav->next;
						strcpy(trav->name, argv[i]);
					} else {
						fprintf(stderr, "err: could not allocate memory\n");
						free_list(a_token);
						return 1;
					}
				} else {
					a_token=malloc(sizeof(node));
					if(a_token) {
						trav=a_token;
						strcpy(a_token->name, argv[i]);
					} else {
						fprintf(stderr, "err: could not allocate memory\n");
						free_list(a_token);
						return 1;
					}
				}
			}
			bad_arg=0;
		}
		
		if( !(strcmp(argv[i], "-h") && strcmp(argv[i], "--help") )) {
			fprintf(stderr, usage_message, argv[0]);
			fprintf(stderr, help_message);
			return 0;
		}
		
		if( !(strcmp(argv[i], "-s") )) {
			a_strip_cruft=1;
			bad_arg=0;
		}
		
		if( !(strcmp(argv[i], "-f") )) {
			a_ignore_errors=1;
			bad_arg=0;
		}
		
		if( !(strcmp(argv[i], "-a") )) {
			a_archived=0x80;
			bad_arg=0;
		}
		
		if( !(strcmp(argv[i], "-p") )) {
			a_pretty=1;
			bad_arg=0;
		}
		
		if( !(strcmp(argv[i], "-t") )) {
			if(argv[i+1]) {
				i++;
				/* convert it to lowercase, so things like "Axe" will be recognised */
				for(j=0; argv[i][j]; j++)
					argv[i][j]=tolower(argv[i][j]);
				if( !(strcmp(argv[i], "basic") )) {
					bad_arg=0;
				}
				if( !(strcmp(argv[i], "axe") )) {
					a_t_set=AXE;
					bad_arg=0;
				}
				if( !(strcmp(argv[i], "grammer") )) {
					a_t_set=GRAMMER;
					bad_arg=0;
				}
			}
		}
		
		if( !(strcmp(argv[i], "-o") )) {
			if(argv[i+1]) {
				i++;
				if(strncmp(argv[i], "-", 1)) {
					a_ofilename=argv[i];
					bad_arg=0;
				}
			}
		}
		
		if( !(strcmp(argv[i], "-i") )) {
			if(argv[i+1]) {
				i++;
				if(strncmp(argv[i], "-", 1)) {
					a_ifilename=argv[i];
					bad_arg=0;
				}
			}
		}
		
		if( !(strcmp(argv[i], "-n") )) {
			if(argv[i+1]) {
				i++;
				if(strncmp(argv[i], "-", 1)) {
					if(strlen(argv[i]) > 8)
						argv[i][8]=0;
					strcpy(&(a_internal_name[0]), argv[i]);
					bad_arg=0;
				}
			}
		}
		
		if(bad_arg)
			break;
		i++;
	}
	
	if(bad_arg) {
		fprintf(stderr, "err: bad option\n");
		return 1;
	}
	
	/* use a pretty token set instead! */
	if(a_pretty)
		a_t_set=a_t_set+NUMBER_OF_SETS;
	
	/* if just a string was passed, try to find a token match! */
	if(a_token) {
		trav=a_token;
		while(trav) {
			for(i=0; i<NUMBER_OF_SETS*2; i++) {
				o_buffer=match_string(i, trav->name, strlen(trav->name), 0);
				if(o_buffer) {
					/* special check, necessary because i convert
					 * ... from source files to \... to prevent
					 * they're becoming an ellipsis upon converting
					 * back */
					if( !strcmp(o_buffer->name, "\\...") )
						strcpy(o_buffer->name, "...");
					if( !strcmp(o_buffer->name, trav->name) ) {
						printf("\"%s\":%s:", o_buffer->name, set_names[i]);
						if(o_buffer->b_first < 16)
							printf("0");
						printf("%X", o_buffer->b_first);
						if(o_buffer->b_second != NONE) {
							if(o_buffer->b_second < 16)
								printf("0");
							printf("%X", o_buffer->b_second);
						}
					puts("");
					}
				free_node(o_buffer);
				}
			}
			trav=trav->next;
		}
		free_list(a_token);
		return 0;
	}
	
/* ----------------------[ FILE READING ]---------------------- */
	
	/* are we reading from a file or from stdin? */
	if(a_ifilename == NULL) {
		i_file=stdin;
		i_buffer.name="stdin";
		i_buffer.bpath=NULL;
		i_buffer.rpath=NULL;
	} else {
		/* grab the path and base name of the main source file */
		i_buffer.rpath=NULL;
		i_swapbuffer=strrchr(a_ifilename, '/');
		if(i_swapbuffer == NULL) {
			i_buffer.name=malloc(sizeof(a_ifilename));
			strcpy(i_buffer.name, a_ifilename);
			i_buffer.bpath=NULL;
		} else {
			i_buffer.name=malloc(sizeof(a_ifilename)-(i_swapbuffer-a_ifilename)-1);
			strcpy(i_buffer.name, i_swapbuffer+1);
			i_buffer.bpath=malloc(i_swapbuffer-a_ifilename+1);
			strncpy(i_buffer.bpath, a_ifilename, i_swapbuffer-a_ifilename+1);
		}		
		
		i_file=fopen(a_ifilename, "r");
		if(!i_file) {
			fprintf(stderr, "err: could not read \"");
			if(i_buffer.bpath != NULL)
				fprintf(stderr, "%s", i_buffer.bpath);
			if(i_buffer.rpath != NULL)
				fprintf(stderr, "%s", i_buffer.rpath);
			fprintf(stderr, "%s\"\n", i_buffer.name);
			
			free_buffer(&i_buffer);
			return 1;
		}
	}
	
	/* dynamic buffering of input file! (necessary for stdin) */
	i_buffer.size=0;
	do {
		if(!realloc_check(&i_buffer)) {
			fclose(i_file);
			return 1;
		}
		swapchar=getc(i_file);
		if(swapchar != EOF) {
			i_buffer.dat[i_buffer.size]=(uint8_t)swapchar;
			i_buffer.size++;
		}
		/* check if the input is in .8xp format */
		if( !strncmp(i_buffer.dat, "**TI83F*", 8) && !(i_buffer.size-8) ) {
			operation_type_flag=0;
			for(j=0; j<45; j++) {
				swapchar=getc(i_file);
				i_buffer.dat[i_buffer.size]=(uint8_t)swapchar;
				i_buffer.size++;
			}
			swapchar=getc(i_file);
			i_buffer.dat[i_buffer.size]=(uint8_t)swapchar;
			k=(uint8_t)swapchar;
			i_buffer.size++;
			swapchar=getc(i_file);
			i_buffer.dat[i_buffer.size]=(uint8_t)swapchar;
			k+=((uint8_t)swapchar)*0x100;
			i_buffer.size++;
			
			for(j=0; j<k; j++) {
				swapchar=getc(i_file);
				if(!realloc_check(&i_buffer)) {
					fclose(i_file);
					return 1;
				}
				i_buffer.dat[i_buffer.size]=(uint8_t)swapchar;
				i_buffer.size++;
			}
			swapchar=EOF;
		}
		
		if(i_buffer.size>=0x80000) {
			fprintf(stderr, "err: obscene input file size\n");
			free_buffer(&i_buffer);
			fclose(i_file);
			return 1;
		}
	} while(swapchar != EOF);
	fclose(i_file);

/* ----------------------[ FILE PARSING ]---------------------- */
	
	if( !operation_type_flag ) {
		if(i_buffer.size < 74) {
			fprintf(stderr, "err: invalid .8xp size\n");
			free_buffer(&i_buffer);
			return 1;
		}
		o_buffer=detokenise(a_t_set, &i_buffer);
	} else {
		
		o_buffer=parse(a_t_set, &i_buffer, a_strip_cruft, a_ignore_errors);
		
		/* "pre-processor directives" here, post
		 * the actual processing */

	}
	if( !o_buffer ) {
		free_buffer(&i_buffer);
		return 1;
	}
	
	
/* ----------------------[ FILE WRITING ]--------------------- */
	
	if(operation_type_flag) {
		var_init(h_point, o_buffer, &(a_internal_name[0]), a_archived);
		header_init(h_point, o_buffer);
	}
	
	if(a_ofilename == NULL) {
		if(operation_type_flag) {
			/* simply print the file's contents here */
			for(i=0; i<76; i++) {
				printf("%c", h_point->top[i]);
				if(i==52 || i==70)
					i++;
			}
		}
		trav=o_buffer;
		while(trav) {
			if(operation_type_flag) {
			printf("%c", trav->b_first);
			if(trav->b_second != NONE)
				printf("%c", trav->b_second);
			} else {
				printf("%s", trav->name);
			}
			trav=trav->next;
		}
		if(operation_type_flag) {
			printf("%c%c", h_point->top[76], h_point->top[77]);
		}
	} else {
		/* open a file and write to it */
		o_file=fopen(a_ofilename, "w");
		
		if(operation_type_flag) {
			/* write header to file */
			fwrite(h_point->top, 1, 11, o_file);
			fwrite(h_point->comment, 1, 42, o_file);
			fwrite(&(h_point->length), 1, 2, o_file);
			/* writing data section here */
			fwrite(&(h_point->var.top), 1, 2, o_file);
			fwrite(&(h_point->var.length), 1, 2, o_file);
			fwrite(&(h_point->var.type), 1, 1, o_file);
			fwrite(&(h_point->var.name), 1, 9, o_file);
			fwrite(&(h_point->var.archived), 1, 1, o_file);
			fwrite(&(h_point->var.length2), 1, 2, o_file);
			fwrite(&(h_point->var.length3), 1, 2, o_file);
		}
		
		/* write data contents here */
		trav=o_buffer;
		while(trav) {
			if(operation_type_flag) {
				fwrite(&(trav->b_first), 1, 1, o_file);
				if(trav->b_second != NONE)
				fwrite(&(trav->b_second), 1, 1, o_file);
			} else {
				fwrite(&(trav->name), 1, strlen(trav->name), o_file);
			}
			trav=trav->next;
		}
		
		if(operation_type_flag) {
			fwrite(&(h_point->checksum), 1, 2, o_file);
		}
			
		fclose(o_file);
	}
	
	free_list(o_buffer);
	free_buffer(&i_buffer);

	return 0;
}

void var_init(header *h, node *list_head, char *a_name, uint8_t a_archived) {
	h->var.top=0x0D;
	h->var.length=get_list_length(list_head)+0x02;
	h->var.type=0x05; /* manually assign to program type */
	strcpy((char*)h->var.name, a_name);
	h->var.archived=a_archived;
	h->var.length2=h->var.length;
	h->var.length3=h->var.length-0x02;
}

void header_init(header *h, node *list_head) {
	uint16_t i;
	uint8_t *p;
	
	static uint8_t new_top[11]={0x2A, 0x2A, 0x54, 0x49, 0x38, 0x33, 0x46, 0x2A, 0x1A, 0x0A, 0x00};
	memcpy(h->top, new_top, sizeof(new_top));
	static char new_comment[42]="generated by tok8x";
	memcpy(h->comment, new_comment, sizeof(new_comment));
	h->length=get_list_length(list_head)+0x13;
	
	/* the checksum is calculated by adding up all of the var's contents and chopping off the upper two bytes */ 
	p=(uint8_t*)&(h->var);
	h->checksum=0;
	for(i=0; i<19; i++) {
		h->checksum+=p[i];
	}
	
	node *trav=list_head;
	while(trav) {	
		h->checksum+=trav->b_first;
		if(trav->b_second != NONE)
			h->checksum+=trav->b_second;
		trav=trav->next;
	}
	
}

uint16_t get_list_length(node *list_head) {
	uint16_t r_length=0;
	while(list_head) {
		if(list_head->b_second != NONE)
			r_length++;
		list_head=list_head->next;
		r_length++;
	}
	return r_length;
}

int realloc_check(buffer *b) {
	char *bs;
	if( !(b->size%2048) ) {
		bs=realloc(b->dat, sizeof(char)*(b->size+2048));
		if(bs == NULL) {
			fprintf(stderr, "err: could not allocate memory. perhaps input is too large?\n");
			free_buffer(b);
			return 0;
		}
		b->dat=bs;
	}
	return 1;
}

void free_node(node *n) {
	if(n != NULL) {
		if(n->val != NULL)
			free(n->val);
		free(n);
	}
}

void free_list(node *list_head) {
	node *temp;
	while(list_head) {
		temp=list_head;
		list_head=list_head->next;
		free_node(temp);
	}
}

void free_buffer(buffer *b) {
	if(b != NULL) {
		free(b->dat);
		if(b->bpath != NULL)
			free(b->bpath);
		if(b->rpath != NULL)
			free(b->rpath);
		if(b->name != NULL)
			free(b->name);
		free(b);
	}
}
