#include "tok8x.h"
#include "tokens.h"

void list_all(int set) {
	int i;
	
	for(i=0; i<t_list_lengths[set]; i++) {
		printf("(%u, %u, %s)\n", t_lists[set][i].b_first, t_lists[set][i].b_second, t_lists[set][i].name);
	}

}
