#ifndef TOK8X_OPT_H
#define TOK8X_OPT_H

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <popt.h>
#include "err.h"

typedef struct opt_s opt_t;
struct opt_s {
	char *token_set;
	char *input;
	char *output;
	char *name;
	bool archived;
	bool pretty;
	bool strip;
};

opt_t* opt_read(int argc, const char *argv[]);
void opt_free(opt_t *o);

#endif
