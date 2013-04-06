#include "tokens.h"

/* build a linked list from token matches */
t_node* tokenise(int set, char buffer[], const uint32_t buffer_size, int ignore_comments, int ignore_errors) {
	uint32_t i=0, column=0, row=0;
	t_node *list_head=NULL, *traverse;
	while(i < buffer_size) {
		if(!list_head) {
			list_head=t_match(set, buffer, buffer_size, i);
			if( !strcmp(list_head->name, "< err >") ) {
				if(ignore_errors) {
					free(list_head);
					list_head=NULL;
					i++;
					column++;
				} else {
					puts("err: unrecognised token at 0:0");
					free(list_head);
					return NULL;
				}
			} else {
				traverse=list_head;
				i+=strlen(traverse->name);
				column+=strlen(traverse->name);
				if( !strcmp(traverse->name, "\n") ) {
					column=0;
					row++;
				}
			}
		} else {
			traverse->next=t_match(set, buffer, buffer_size, i);
			if( !strcmp(traverse->next->name, "< err >") ) {
				if(ignore_errors) {
					free(traverse->next);
					traverse->next=NULL;
					i++;
					column++;
				} else {
					printf("%u:%u: err: unrecognised token\n", row+1, column+1);
					free_list(list_head);
					return NULL;
				}
			} else {
				traverse=traverse->next;
				i+=strlen(traverse->name);
				column+=strlen(traverse->name);
				if( !strcmp(traverse->name, "\n") ) {
					column=0;
					row++;
				}
			}
		}
	}
	return list_head;
}

t_node* t_match(int set, char buffer[], const uint32_t buffer_size, const int cursor) {
	t_node *rp=malloc(sizeof(t_node));
	strcpy(rp->name, "");
	
	/* match a token here */
	/* find the longest complete token match for a substring in buffer which
	 * begins at the current cursor position */
	int i, match=0, length_left=buffer_size-cursor;
	for(i=0; i<t_list_lengths[set]; i++) {
		/* if the first character matches */
		if( !strncmp(&buffer[cursor], t_lists[set][i].name, 1) ) {
			/* if the potential match isn't too long to fit in
			 * what remains of the file */
			if( strlen(t_lists[set][i].name) <= length_left ) {				
				/* check if the two strings are actually equal */
				if( !strncmp(&buffer[cursor], t_lists[set][i].name, strlen(t_lists[set][i].name)) ) {
					/* check if the length of the found string is greater than the one already present */
					if( strlen(t_lists[set][i].name) > strlen(rp->name) ) {
						/* we found a match! */
						match=1;
						rp->b_first=t_lists[set][i].b_first;
						rp->b_second=t_lists[set][i].b_second;
						strcpy(rp->name, t_lists[set][i].name);
					}
				}
				
			}
		}
	}
	
	if(!match)
		strcpy(rp->name, "< err >");
	rp->next=NULL;
	 
	return rp;
}
