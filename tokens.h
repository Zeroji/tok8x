#include "tok8x.h"

#define NONE 0xFF

typedef struct token {
	uint8_t b_first;
	uint8_t b_second;
	const char *name[];
}
