#include "tok8x.h"

typedef struct string {
	uint32_t start;
	uint32_t length;
	char *val;
	uint32_t end;
} string;

extern t_node* preprocess(t_node *b);
extern t_node* define(t_node *b);
extern t_node* include(t_node *b);
extern t_node* conditional(t_node *b);
