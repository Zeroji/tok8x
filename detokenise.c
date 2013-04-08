#include "tokens.h"

/* this should be a fairly simple process of checking
 * in t_2byte_indicators to see if it should be looking
 * for the second byte as well and then searching
 * for a match in t_lists[set], so i'm saving it
 * for last */
t_node* detokenise(int set, char buffer[], const uint32_t buffer_size) {
	uint32_t i, column=0, row=0;;
	t_node *list_head=NULL, *traverse;
	for(i=74; i<buffer_size-1; i++) {
		if(!list_head) {
			list_head=match_token(set, buffer, buffer_size, i);
			if(!list_head) {
				puts("0:0: err: unrecognised token");
				free(list_head);
				return NULL;
			}
			column++;
			if(list_head->b_first == 0x3F) {
				column=0;
				row++;
			}
			traverse=list_head;
		} else {
			traverse->next=match_token(set, buffer, buffer_size, i);
			if(!traverse->next) {
				printf("%u:%u: err: unrecognised token\n", row+1, column+1);
				free_list(list_head);
				return NULL;
			}
			traverse=traverse->next;
			column++;
			if(traverse->b_first == 0x3F) {
				column=0;
				row++;
			}
		}
		if(traverse->b_second != NONE)
			i++;
	}
	return list_head;
}

t_node* match_token(int set, char buffer[], const uint32_t buffer_size, const int cursor) {
	int i;
	t_node *rp=malloc(sizeof(t_node));
	rp->b_first=buffer[cursor];
	rp->b_second=NONE;
	
	for(i=0; i<11; i++) {
		if(rp->b_first == t_2byte_indicators[i]) {
			if(cursor == buffer_size)
				return NULL;
			rp->b_second=buffer[cursor+1];
		}
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
