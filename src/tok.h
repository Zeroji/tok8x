#ifndef TOK8X_TOK_H
#define TOK8X_TOK_H

#include <stdint.h>
#include <stdbool.h>
#include <uthash.h>

#define NONE 0xFF
#define TOKLEN 20

/* not using anonymous union / 
 * struct here to save space because
 * it would make initialising the token
 * lists all messy and things */
typedef struct tok_s tok_t;
struct tok_s {
	uint8_t b1;
	uint8_t b2;
	char name[TOKLEN];
	tok_t *subhash;
	bool is_subhash;
	UT_hash_handle hht, hhs;
};

typedef enum {
	BASIC = 0,
	AXE = 1,
	GRAMMER = 2,
	PRETTY_BASIC = 3,
	PRETTY_AXE = 4,
	PRETTY_GRAMMER = 5,
	LIST_COUNT = 6,
} t_list_t;

extern int t_list_lengths[];
extern tok_t *t_lists[];

bool t_is2byte(uint8_t y);

#endif
