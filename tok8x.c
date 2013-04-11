#include "tok8x.h"

int main(int argc, char **argv) {
	uint32_t i, j, k, bad_arg;
	uint32_t if_size;
	char swapchar;
	header h;
	struct header *h_point=&h;
	FILE *i_file;
	char *i_buffer=NULL;
	char *i_swapbuffer;
	FILE *o_file;
	t_node *o_buffer;
	t_node *traverse;
	int operation_type_flag=1; /* set to "tokenising" as default. if we later find that the
								* input was an 8x file, we'll set it to "detokenising" */
	
	/* input argument vars */
	char *a_ifilename=NULL;
	char *a_ofilename=NULL;
	t_node *a_token=NULL;
	int a_strip_cruft=0; /* axe one line comments start with \n., grammer with // 
								axe multi-line comments use ... (be sure to ignore
								conditional comments (...If, ...!If, ...Else ) */
	int a_ignore_errors=0;
	uint8_t a_archived=0x00;
	t_set a_t_set=BASIC;
	char a_internal_name[9]={0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	char *help_message="\noptions:\n -h\n   show this help dialogue\n\n -t <axe|basic|grammer>\n   define token set to be used\n\n -i <filename>\n   define file to be read from\n\n -o <filename>\n   define file to be written (defaults to out.[txt|8xp])\n\n -n <name>\n   define on-calc name (warning: does not check name validity)\n\n -a\n   generate archived program\n\n -s\n   strip excess data (comments, spaces, empty lines, etc)\n\n -f\n   force (skip over any unmatched tokens rather than generating an error)\n\n";
	char *usage_message="usage:\n %s -i <filename> [options]\n %s <token name>\n";
	
/* ----------------------[ INPUT PARSING ]---------------------- */
	
	i=1;
	while(argv[i]){
		bad_arg=1;
		if(strncmp(argv[i], "-", 1)) {
			if( !(strlen(argv[i]) > 16) ) {
				if(a_token) {
					traverse->next=malloc(sizeof(t_node));
					if(traverse->next) {
						traverse=traverse->next;
						strcpy(traverse->name, argv[i]);
					} else {
						fprintf(stderr, "err: could not allocate memory\n");
						free_list(a_token);
						return 1;
					}
				} else {
					a_token=malloc(sizeof(t_node));
					if(a_token) {
						traverse=a_token;
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
				if( !(strcmp(argv[i], "pretty") )) {
					a_t_set=PRETTY;
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
	
	
/* if just a string was passed, try to find a token match! */
	if(a_token) {
		traverse=a_token;
		while(traverse) {
			for(i=0; i<4; i++) {
				o_buffer=match_string(i, traverse->name, strlen(traverse->name), 0);
				if(o_buffer) {
					if( !strcmp(o_buffer->name, traverse->name) ) {
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
					free(o_buffer);
				}
			}
			traverse=traverse->next;
		}
		free_list(a_token);
		return 0;
	}
	
/* ----------------------[ FILE READING ]---------------------- */
		
	/* are we reading from a file or from stdin? */
	if(a_ifilename == NULL) {
		i_file=stdin;
	} else {
		i_file=fopen(a_ifilename, "r");
		if(!i_file) {
			fprintf(stderr, "err: could not read \"%s\"\n", a_ifilename);
			return 1;
		}
	}
	
	/* dynamic buffering of input file! (necessary for stdin) */
	if_size=0;
	do {
		if( !(if_size%256) ) {
			i_swapbuffer=realloc(i_buffer, sizeof(char)*(if_size+256));
			if(i_swapbuffer == NULL) {
				fprintf(stderr, "err: could not allocate memory\n");
				free(i_buffer);
				fclose(i_file);
				return 1;
			}
			i_buffer=i_swapbuffer;
		}
		swapchar=getc(i_file);
		if(swapchar != EOF) {
			i_buffer[if_size]=(uint8_t)swapchar;
			if_size++;
		}
		/* check if the input is in .8xp format */
		if( !strncmp(i_buffer, "**TI83F*", 8) && !(if_size-8) ) {
			operation_type_flag=0;
			for(j=0; j<45; j++) {
				swapchar=getc(i_file);
				i_buffer[if_size]=(uint8_t)swapchar;
				if_size++;
			}
			swapchar=getc(i_file);
			i_buffer[if_size]=(uint8_t)swapchar;
			k=(uint8_t)swapchar;
			if_size++;
			swapchar=getc(i_file);
			i_buffer[if_size]=(uint8_t)swapchar;
			k+=((uint8_t)swapchar)*0x100;
			if_size++;
			
			for(j=0; j<k+2; j++) {
				swapchar=getc(i_file);
				i_buffer[if_size]=(uint8_t)swapchar;
				if_size++;
			}
			swapchar=EOF;
		}
		
		if( !(if_size-0x80000) ) {
			fprintf(stderr, "err: obscene input file size\n");
			free(i_buffer);
			fclose(i_file);
			return 1;
		}
	} while(swapchar != EOF);
	if_size--;
	fclose(i_file);

/* ----------------------[ FILE PARSING ]---------------------- */
	
	if( !operation_type_flag ) {
		if(if_size < 74) {
			fprintf(stderr, "err: invalid .8xp size\n");
			free(i_buffer);
			return 1;
		}
		o_buffer=detokenise(a_t_set, i_buffer, if_size);
	} else {
		
		/* pre-processor directives here */
		
		o_buffer=tokenise(a_t_set, i_buffer, if_size, a_strip_cruft, a_ignore_errors);
	}
	if( !o_buffer ) {
		free(i_buffer);
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
		traverse=o_buffer;
		while(traverse) {
			if(operation_type_flag) {
			printf("%c", traverse->b_first);
			if(traverse->b_second != NONE)
				printf("%c", traverse->b_second);
			} else {
				printf("%s", traverse->name);
			}
			traverse=traverse->next;
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
		traverse=o_buffer;
		while(traverse) {
			if(operation_type_flag) {
				fwrite(&(traverse->b_first), 1, 1, o_file);
				if(traverse->b_second != NONE)
				fwrite(&(traverse->b_second), 1, 1, o_file);
			} else {
				fwrite(&(traverse->name), 1, strlen(traverse->name), o_file);
			}
			traverse=traverse->next;
		}
		
		if(operation_type_flag) {
			fwrite(&(h_point->checksum), 1, 2, o_file);
		}
			
		fclose(o_file);
	}
	
	free_list(o_buffer);
	free(i_buffer);

	return 0;
}

void var_init(header *h, t_node *list_head, char *a_name, uint8_t a_archived) {
	h->var.top=0x0D;
	h->var.length=get_list_length(list_head)+0x02;
	h->var.type=0x05; /* manually assign to program type */
	strcpy((char*)h->var.name, a_name);
	h->var.archived=a_archived;
	h->var.length2=h->var.length;
	h->var.length3=h->var.length-0x02;
}

void header_init(header *h, t_node *list_head) {
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
	
	t_node *traverse=list_head;
	while(traverse) {	
		h->checksum+=traverse->b_first;
		if(traverse->b_second != NONE)
			h->checksum+=traverse->b_second;
		traverse=traverse->next;
	}
	
}

void free_list(t_node *list_head) {
	t_node *temp;
	while(list_head) {
		temp=list_head;
		list_head=list_head->next;
		free(temp);
	}
}

uint16_t get_list_length(t_node *list_head) {
	uint16_t r_length=0;
	while(list_head) {
		if(list_head->b_second != NONE)
			r_length++;
		list_head=list_head->next;
		r_length++;
	}
	return r_length;
}
