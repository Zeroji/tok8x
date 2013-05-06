#include "parse.h"

/* redo this, including AFTER tokenising the main file, so that
 * line numbers actually make sense. make file paths relative to
 * the main source file, too, rather than the program, and use
 * "s to surround the name so that file names with spaces are
 * understood */

node* parse(int set, buffer *b, int strip_cruft, int ignore_errors) {
	node *t;

	if(!b) {
		return NULL;
	}
	//~ b=define(b);
	if(!b)
		return NULL;
		
	t=tokenise(set, b, strip_cruft, ignore_errors);
	
	//~ t=conditional(t);
	//~ t=include(t);
	
	return t;
}

buffer* define(buffer *b) {
	uint32_t i=0, column=0, row=0, markc, markr, marki;
	buffer *tempbuff;
	node *list_head=NULL, *trav;
	
	if(!b)
		return NULL;
	
	while(i < b->size-8) {

		if(b->dat[i] == '#') {
			if( !strncmp(&(b->dat[i]), "##define", 8) ) {
				marki=i;
				markc=column;
				markr=row;
				i+=8;
				/* ##define found. now grab the two args */
				/* grab the first */
				if(list_head == NULL) {
					list_head=get_word(b, &i, markr, markc, "##define");
					trav=list_head;
					list_head->next=NULL;
					if(list_head == NULL) {
						free_node(list_head);
						return NULL;
					}
				} else {
					trav->next=get_word(b, &i, markr, markc, "##define");
					trav=trav->next;
					if(trav == NULL) {
						free_list(list_head);
						return NULL;
					}
					trav->next=NULL;
				}
				/* grab the second */
				trav->next=get_word(b, &i, markr, markc, "##define");
				trav=trav->next;
				if(trav == NULL) {
					free_list(list_head);
					return NULL;
				}
				/* cut the line out of the buffer */
				memmove(&(b->dat[marki]), &(b->dat[i]), b->size-i);
				b->size=b->size-(i-marki);
				tempbuff=realloc(b->dat, b->size);
				if(tempbuff == NULL) {
					free_list(list_head);
					return NULL;
				}
			} else {
				i++;
				column++;
			}
		} else {
			if(b->dat[i] == '\n') {
				column=0;
				row++;
			} else {
				column++;
			}
			i++;

		}
	}
	
	/* alrighty, now use the linked list of words to perform substitutions */
	
	free_list(list_head);
		
	return b;
}

node* include(node *t) {
	if(!t)
		return NULL;
	return t;
}

node* conditional(node *t) {
	if(!t)
		return NULL;
	return t;
}

/* grab and return a pointer to a string between two "s */
extern node* get_word(buffer *b, uint32_t *i, uint32_t row, uint32_t column, const char *opname) {
	uint32_t start;
	node* r_word;
	
	r_word=malloc(sizeof(node));
	r_word->next=NULL;
	
	while(b->dat[*i] != '\"' && b->dat[*i] != '\n' && *i < b->size-1) {
		column++;
		(*i)++;
	}
	if(b->dat[*i] != '\"') {
		if(b->rpath != NULL)
			fprintf(stderr, "%s", b->rpath);
		fprintf(stderr, "%s:%u:%u: err: bad %s\n", b->name, row+1, column+1, opname);
		return NULL;
	}
	/* i now points to the first " */
	(*i)++;
	start=*i;
	while(b->dat[*i] != '\"' && b->dat[*i] != '\n' && *i < b->size-1) {
		column++;
		(*i)++;
	}
	if(b->dat[*i] != '\"' || (*i) == start) {
		if(b->rpath != NULL)
			fprintf(stderr, "%s", b->rpath);
		fprintf(stderr, "%s:%u:%u: err: bad %s\n", b->name, row+1, column+1, opname);
		return NULL;
	}
	/* i now points to the second " */
	r_word->val=malloc(sizeof(char)*((*i)-start));
	if(r_word->val == NULL) {
		fprintf(stderr, "err: memory allocation failed\n");
		return NULL;
	}
	strncpy(r_word->val, &(b->dat[start]), (*i)-start);
	(*i)++;
	if(*i < b->size-1) {
		if(b->dat[*i] == '\n')
			(*i)++;
		if(*i-1 < b->size-strlen(opname)) {
			fprintf(stderr, "poke!\n");
			if( !strncmp(&(b->dat[(*i)-1]), opname, strlen(opname)) )
				(*i)--;
		}
	}
	return r_word;
}
