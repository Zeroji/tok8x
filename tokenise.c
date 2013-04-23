#include "tokens.h"

/* build a linked list from token matches */
t_node* tokenise(int set, buffer *b, int strip_cruft, int ignore_errors) {
	uint32_t i=0, column=0, row=0;
	t_node *list_head=NULL, *trav, *temp, *temp2;
	while(i < b->size-1) {
		if(!list_head) {
			list_head=match_string(PREPROC, b->dat, b->size, i);
			/* check if the token is a preprocessor directive. if
			 * not, check if it's in the defined set. if not, and
			 * the set is not BASIC, check in BASIC */
			if(!list_head) {
				list_head=match_string(set, b->dat, b->size, i);
			}
			
			if(!list_head && set != BASIC) {
				list_head=match_string(BASIC, b->dat, b->size, i);
			}
			
			if(!list_head) {
				if(ignore_errors) {
					i++;
					column++;
				} else {
					if(b->name) {
						if(b->rpath)
							fprintf(stderr, "%s", b->rpath);
						fprintf(stderr, "%s:1:1: err: unrecognised token\n", b->name);
					} else {
						fprintf(stderr, "stdin:1:1: err: unrecognised token\n");
					}
					return NULL;
				}
			} else {
				trav=list_head;
				i+=strlen(trav->name);
				column+=strlen(trav->name);
				if( !strcmp(trav->name, "\n") ) {
					column=0;
					row++;
				}
			}
		} else {
			trav->next=match_string(PREPROC, b->dat, b->size, i);
			
			if(!trav->next) {
				trav->next=match_string(set, b->dat, b->size, i);
			}
			
			if(!trav->next && set != BASIC) {
				trav->next=match_string(BASIC, b->dat, b->size, i);
			}
			
			if(!trav->next) {
				if(ignore_errors) {
					i++;
					column++;
				} else {
					if(b->name) {
						if(b->rpath)
							fprintf(stderr, "%s", b->rpath);
						fprintf(stderr, "%s:%u:%u: err: unrecognised token\n", b->name, row+1, column+1);
					} else {
						fprintf(stderr, "stdin:%u:%u: err: unrecognised token\n", row+1, column+1);
					}
					free_list(list_head);
					return NULL;
				}
			} else {
				trav=trav->next;
				i+=strlen(trav->name);
				column+=strlen(trav->name);
				if( !strcmp(trav->name, "\n") ) {
					column=0;
					row++;
				}
			}
		}
	}
	
	/* make a second pass to remove cruft, including
	 * leading spaces, empty lines, and comments */
	if(strip_cruft) {
		trav=list_head;
		/* i=0 : currently outside of comment
		 * 1=1 : currently inside a string
		 * 1=2 : in axe one line comment
		 * i=3 : in axe multiline comment
		 * i=4 : in grammer comment */
		i=0;
		while(trav) {
			/* if the current token is a " */
			if(trav->b_first == 0x2A) {
				if(i==0) {
					i=1;
				} else {
					i=0;
				}
			}
						
			/* if not currently in a string */
			if(i==0) {
				if(trav->next) {
					/* if next token is at a space */
					if(trav->next->b_first == 0x29) {
						temp=trav->next;
						trav->next=trav->next->next;
						free(temp);
					}
					/* if next token is at an empty line */
					if(trav->next->next) {
						if(trav->next->b_first == 0x3F && trav->next->next->b_first == 0x3F) {
							temp=trav->next;
							trav->next=trav->next->next;
							free(temp);
						}						
				
						if(set == AXE) {
							/* strip out one line comments, which begin with \n. and end with \n */
							if(trav->next->b_first == 0x3F && trav->next->next->b_first == 0x3A) {
								temp=trav;
								trav=trav->next;
								while(trav->next) {
									temp2=trav;
									trav=trav->next;
									free(temp2);
									if(trav->b_first == 0x3F) {
										temp->next=trav->next;
										free(trav);
										trav=temp;
										break;
									}
								}
							}
							
							/* strip out multi-line comments, which begin with \n... and end with ...\n */
							if(trav->next->b_first == 0x3F && trav->next->next->b_first == 0xBB && trav->next->next->b_second == 0xDB) {
								temp=trav;
								trav=trav->next->next;
								free(temp->next);
								while(trav->next) {
									temp2=trav;
									trav=trav->next;
									free(temp2);
									if(trav->next->b_first == 0x3f && trav->b_first == 0xBB && trav->b_second == 0xDB) {
										temp->next=trav->next->next;
										free(trav->next);
										free(trav);
										trav=temp;
										break;
									}
								}
							}
							
						}
						
						if(set == GRAMMER) {
							/* strip out one line comments, which begin with // */
							if(trav->next->b_first == 0x83 && trav->next->next->b_first == 0x83) {
								temp=trav;
								trav=trav->next;
								while(trav->next) {
									temp2=trav;
									trav=trav->next;
									free(temp2);
									if(trav->b_first == 0x3F) {
										temp->next=trav->next;
										free(trav);
										trav=temp;
										break;
									}
								}
							}
							
						}
					}
				}
			}
			trav=trav->next;
		}
	}
	
	return list_head;
}

t_node* match_string(int set, char buff[], const uint32_t buff_size, uint32_t cursor) {
	t_node *rp=malloc(sizeof(t_node));
	strcpy(rp->name, "");
	
	/* match a token here */
	/* find the longest complete token match for a substring in buffer which
	 * begins at the current cursor position */
	int i, match=0, length_left=buff_size-cursor;
	for(i=0; i<t_list_lengths[set]; i++) {
		/* if the first character matches */
		if( !strncmp(&buff[cursor], t_lists[set][i].name, 1) ) {
			/* if the potential match isn't too long to fit in
			 * what remains of the file */
			if( strlen(t_lists[set][i].name) <= length_left ) {				
				/* check if the two strings are actually equal */
				if( !strncmp(&buff[cursor], t_lists[set][i].name, strlen(t_lists[set][i].name)) ) {
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
