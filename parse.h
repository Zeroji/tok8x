#include "tok8x.h"

extern buffer* define(buffer *b);
extern tnode* include(tnode *t);
extern tnode* conditional(tnode *t);

extern wnode* get_word(buffer *b, uint32_t *i, uint32_t row, uint32_t column, const char *opname);
