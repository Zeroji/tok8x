#include "tokens.h"

/* build a linked list from token matches */
t_node* tokenise(int set, char buffer[], const uint32_t buffer_size, int strip_cruft, int ignore_errors) {
	uint32_t i=0, column=0, row=0;
	t_node *list_head=NULL, *traverse, *temp;
	while(i < buffer_size) {
		if(!list_head) {
			list_head=match_string(set, buffer, buffer_size, i);
			/* if there is no match in the current set, check
			 * the default BASIC set instead */
			if(set != 0) {
				temp=match_string(0, buffer, buffer_size, i);
				if(temp) {
					if(!list_head) {
						list_head=temp;
					} else {
						free(temp);
					}
				}
			}
			
			if(!list_head) {
				if(ignore_errors) {
					i++;
					column++;
				} else {
					fprintf(stderr, "0:0: err: unrecognised token\n");
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
			traverse->next=match_string(set, buffer, buffer_size, i);
			
			if(set != 0) {
				temp=match_string(0, buffer, buffer_size, i);
				if(temp) {
					if(!traverse->next) {
						traverse->next=temp;
					} else {
						free(temp);
					}
				}
			}
			
			if(!traverse->next) {
				if(ignore_errors) {
					i++;
					column++;
				} else {
					fprintf(stderr, "%u:%u: err: unrecognised token\n", row+1, column+1);
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
	
	/* make a second pass to remove cruft, including
	 * leading spaces, empty lines, and comments */
	
	if(strip_cruft) {
		traverse=list_head;
		/* i=0 : currently outside of comment
		 * 1=1 : currently inside a string
		 * 1=2 : in axe one line comment
		 * i=3 : in axe multiline comment
		 * i=4 : in grammer comment */
		i=0;
		while(traverse) {
			/* if the current token is a " */
			if(traverse->b_first == 0x2A) {
				if(i==0) {
					i=1;
				} else {
					i=0;
				}
			}
						
			/* if not currently in a string */
			if(i==0) {
				if(traverse->next) {
					/* if next token is at a space */
					if(traverse->next->b_first == 0x29) {
						temp=traverse->next;
						traverse->next=traverse->next->next;
						free(temp);
					}
					/* if next token is at an empty line */
					if(traverse->next->next) {
						if(traverse->next->b_first == 0x3F && traverse->next->next->b_first == 0x3F) {
							temp=traverse->next;
							traverse->next=traverse->next->next->next;
							free(temp->next);
							free(temp);
						}
					}						
				}
			}
			traverse=traverse->next;
		}
	}
	
	
	return list_head;
}

t_node* match_string(int set, char buffer[], const uint32_t buffer_size, const int cursor) {
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
	
	if(!match) {
		free(rp);
		return NULL;
	}
	 
	return rp;
}
