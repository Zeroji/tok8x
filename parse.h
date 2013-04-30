#include "tok8x.h"

typedef struct string {
	uint32_t start;
	uint32_t length;
	char *val;
	uint32_t end;
} string;

extern buffer* define(buffer *b);
extern t_node* include(t_node *t);
extern t_node* conditional(t_node *t);
