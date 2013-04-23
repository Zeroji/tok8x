#include "tokens.h"

/* this should be a fairly simple process of checking
 * in t_2byte_indicators to see if it should be looking
 * for the second byte as well and then searching
 * for a match in t_lists[set], so i'm saving it
 * for last */
t_node* detokenise(int set, buffer b) {
	uint32_t i, column=0, row=0;
	t_node *list_head=NULL, *trav;
	int flag;
	
	for(i=0x4A /* jump straight to the data section */; i<b.size-1; i++) {
		if(!list_head) {
			
			list_head=match_token(set, b.dat, b.size, i);
			if(!list_head && set !=0)
				list_head=match_token(0, b.dat, b.size, i);
			if(!list_head) {
				if(b.name == NULL) {
					fprintf(stderr, "stdin");
				} else {
					if(b.rpath)
						fprintf(stderr, "%s", b.rpath);
					fprintf(stderr, "%s", b.name);
				}
				fprintf(stderr, ":0:0: err: unrecognised token at \"");
				if(b.dat[i]<0x10)
					fprintf(stderr, "0");
				fprintf(stderr, "%X\"\n", b.dat[i]);
				free(list_head);
				return NULL;
			}
			column++;
			if(list_head->b_first == 0x3F) {
				column=0;
				row++;
			}
			trav=list_head;
			
		} else {
			
			trav->next=match_token(set, b.dat, b.size, i);
			if(!trav->next && set != 0)
				trav->next=match_token(0, b.dat, b.size, i);
			if(!trav->next) {
				if(b.name == NULL) {
					fprintf(stderr, "stdin");
				} else {
					if(b.rpath)
						fprintf(stderr, "%s", b.rpath);
					fprintf(stderr, "%s", b.name);
				}
				fprintf(stderr, ":0:0: err: unrecognised token at \"");
				if(b.dat[i]<0x10)
					fprintf(stderr, "0");
				fprintf(stderr, "%X", b.dat[i]);
				if(i<b.size) {
					if(c_is_2byte(b.dat[i])) {
						if(b.dat[i+1] != 0xFF) {
							if(b.dat[i+1]<0x10)
								fprintf(stderr, "0");
							fprintf(stderr, "%X", b.dat[i+1]);
						}
					}
				}
				fprintf(stderr, "\"\n");
				free_list(list_head);
				return NULL;
			}
			trav=trav->next;
			column++;
			if(trav->b_first == 0x3F) {
				column=0;
				row++;
			}
		}
		
		/* testing */
		fprintf(stderr, "%X: ", i);
		if( trav->b_first< 0x10)
			fprintf(stderr, "0");
		fprintf(stderr, "%X", trav->b_first);
		if( trav->b_second< 0x10)
			fprintf(stderr, "0");
		fprintf(stderr, "%X\n", trav->b_second);
			
		if(trav->b_second != NONE)
			i++;
	}
	
	/* convert spaces at beginnings of lines to tabs
	 * for easier reading */
	trav=list_head;
	flag=0;
	while(trav) {
		if(trav->b_first == 0x3f)
			flag=1;
		trav=trav->next;
		if(trav && flag) {
			if(trav->b_first == 0x29) {
				strcpy(trav->name, "\t");
			} else {
				flag=0;
			}
		}
	}
	return list_head;
}

t_node* match_token(int set, char buff[], const uint32_t buff_size, uint32_t cursor) {
	int i;
	t_node *rp=malloc(sizeof(t_node));
	rp->b_first=buff[cursor];
	rp->b_second=NONE;
	
	if(c_is_2byte(rp->b_first)) {
		if(cursor == buff_size)
			return NULL;
		rp->b_second=buff[cursor+1];
	}
	
	for(i=0; i<t_list_lengths[set]; i++) {
		if(rp->b_first == t_lists[set][i].b_first && rp->b_second == t_lists[set][i].b_second) {
			strcpy(rp->name, t_lists[set][i].name);
			return rp;
		}
	}
	
	/* if no legitimate match was found, return NULL */
	return NULL;
}

/* check if a byte indicates a 2byte token */
extern int c_is_2byte(uint8_t b_first) {
	int i;
	
	for(i=0; i<11; i++) {
		if(b_first == t_2byte_indicators[i]) {
			return 1;
		}
	}
	
	return 0;
}
