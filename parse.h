#include "tok8x.h"

extern buffer* define(buffer *b);
extern node* include(node *t);
extern node* conditional(node *t);

extern node* get_word(buffer *b, uint32_t *i, uint32_t row, uint32_t column, const char *opname);
