#include "tokens.h"

/* this should be a fairly simple process of checking
 * in t_2byte_indicators to see if it should be looking
 * for the second byte as well and then searching
 * for a match in t_lists[set], so i'm saving it
 * for last */
t_node* detokenise(int set, char buffer[], const uint32_t buffer_size) {
	uint32_t i;
	t_node *list_head=NULL, *traverse;
	for(i=0; i<buffer_size; i++) {
	}
	return NULL;
}
