
#ifndef _TOKENS

#include "tok8x.h"

extern int t_list_lengths[];
extern token *t_lists[];

/* sub routine for finding a token match at the current char */
t_node* t_match(int set, char buffer[], const uint32_t buffer_size, const int cursor);

#endif
