#ifndef TOK8X_TOK_H
#define TOK8X_TOK_H

#include <stdint.h>
#include <uthash.h>

#define NONE 0xFF

typedef struct tok_s tok_t;
struct tok_s {
	uint8_t b1;
	uint8_t b2;
	char name[20];
	UT_hash_handle hh1, hh2, hhs;
};

typedef enum {
	BASIC = 0,
	AXE = 1,
	GRAMMER = 2,
	PRETTY_BASIC = 3,
	PRETTY_AXE = 4,
	PRETTY_GRAMMER = 5,
	T_LIST_COUNT = 6,
} T_LIST;


extern int t_list_lengths[];
extern tok_t *t_lists[];
extern uint8_t t_2byte_indicators[];

#endif
