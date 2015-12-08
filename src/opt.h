#ifndef TOK8X_OPT_H
#define TOK8X_OPT_H

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <popt.h>
#include "err.h"
#include "tok.h"

typedef struct opt_s opt_t;
struct opt_s {
	char *list_string;
	t_list_t list;
	char *output;
	char *name;
	bool info;
	bool archived;
	bool pretty;
	bool strip;
	char **extra_args;
};

opt_t* opt_read(int argc, const char *argv[]);
void opt_free(opt_t *o);

#endif
