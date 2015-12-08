#include "opt.h"

enum {
	OPT_HELP = 1,
	OPT_INFO = 2,
	OPT_TOKEN_SET = 3,
	OPT_OUTPUT = 4,
	OPT_NAME = 5,
	OPT_ARCHIVED = 6,
	OPT_PRETTY = 7,
	OPT_STRIP = 8,
};

#define OPT_ERR(format, ...) \
	do { \
		fprintf(stderr, "\e[1mtok8x:\e[0m "); \
		fprintf(stderr, "\e[1;31merror:\e[0m " format "\n", ##__VA_ARGS__); \
		opt_free(o); \
		poptFreeContext(context); \
		exit(EINVAL); \
	} while(0)

const char* set_names[] = {
	"BASIC",
	"Axe",
	"Grammer",
};

/* returns LIST_COUNT if not found. ensures the string
 * has proper casing if found */
static t_list_t list_name2list(char *s)
{
	t_list_t l;
	int i;

	l = BASIC;
	while(l < LIST_COUNT / 2) {
		i = 0;
		while(s[i] != '\0' && set_names[l][i] != '\0') {
			if( tolower(s[i]) != tolower(set_names[l][i]) ) {
				break;
			}
			i++;
		}

		if(s[i] == '\0' && set_names[l][i] == '\0') {
			strcpy(s, set_names[l]);
			return l;
		}

		l++;
	}

	return LIST_COUNT;
}

opt_t* opt_read(int argc, const char *argv[])
{
	int r, i;
	opt_t *o = calloc(1, sizeof(opt_t));
	const char **extra_args;

	EXIT_NULL(o);

	struct poptOption options[] = {
		{
			.longName = "help",
			.shortName = 'h',
			.argInfo = POPT_ARG_NONE,
			.arg = NULL,
			.val = OPT_HELP,
			.descrip = "display this help dialogue",
			.argDescrip = NULL
		},
		{
			.longName = "info",
			.shortName = 'i',
			.argInfo = POPT_ARG_NONE,
			.arg = NULL,
			.val = OPT_INFO,
			.descrip = "print information on tokens rather than converting"
				"files",
			.argDescrip = NULL
		},
		{
			.longName = "token_set",
			.shortName = 't',
			.argInfo = POPT_ARG_STRING,
			.arg = &(o->list_string),
			.val = OPT_TOKEN_SET,
			.descrip = "define token set to be used",
			.argDescrip = "<axe|basic|grammer>"
		},
		{
			.longName = "output",
			.shortName = 'o',
			.argInfo = POPT_ARG_STRING,
			.arg = &(o->output),
			.val = OPT_OUTPUT,
			.descrip = "define file to be written (defaults to stdout)",
			.argDescrip = "<filename>"
		},
		{
			.longName = "name",
			.shortName = 'n',
			.argInfo = POPT_ARG_STRING,
			.arg = &(o->name),
			.val = OPT_NAME,
			.descrip = "define on-calc name (defaults to 'A')",
			.argDescrip = "<name>"
		},
		{
			.longName = "archived",
			.shortName = 'a',
			.argInfo = POPT_ARG_NONE,
			.arg = NULL,
			.val = OPT_ARCHIVED,
			.descrip = "generate archived program.",
			.argDescrip = NULL
		},
		{
			.longName = "pretty",
			.shortName = 'p',
			.argInfo = POPT_ARG_NONE,
			.arg = NULL,
			.val = OPT_PRETTY,
			.descrip = "use \"pretty\" output",
			.argDescrip = NULL
		},
		{
			.longName = "strip",
			.shortName = 's',
			.argInfo = POPT_ARG_NONE,
			.arg = NULL,
			.val = OPT_STRIP,
			.descrip = "strip excess data",
			.argDescrip = NULL
		},
		{ NULL, 0, 0, NULL, 0 }
	};

	poptContext context = poptGetContext("tok8x", argc, argv, options, 0);

	while( (r = poptGetNextOpt(context)) != -1) {
		switch(r) {
			case OPT_TOKEN_SET:
				o->list = list_name2list(o->list_string);
				if(o->list == LIST_COUNT) {
					OPT_ERR("unrecognised token set \"%s\"",
							o->list_string
						   );
				}
				break;

			case OPT_INFO:
				o->info = true;
				if(o->output != NULL || o->name != NULL || o->archived
						|| o->strip) {
					OPT_ERR("-i cannot be used with -o, -n, -a, or -s");
				}
				break;

			case OPT_OUTPUT:
				if(o->info) {
					OPT_ERR("-i cannot be used with -o, -n, -a, or -s");
				}
				break;

			case OPT_NAME:
				if(o->info) {
					OPT_ERR("-i cannot be used with -o, -n, -a, or -s");
				}
				break;

			case OPT_ARCHIVED:
				if(o->info) {
					OPT_ERR("-i cannot be used with -o, -n, -a, or -s");
				}
				o->archived = true;
				break;

			case OPT_PRETTY:
				o->pretty = true;
				break;

			case OPT_STRIP:
				if(o->info) {
					OPT_ERR("-i cannot be used with -o, -n, -a, or -s");
				}
				o->strip = true;
				break;

			default:
				if(r != OPT_HELP) {
					OPT_ERR("\e[1m%s\e[0m: %s",
							poptBadOption(context, 0),
							poptStrerror(r)
						   );
				}

				poptPrintHelp(context, stdout, 0);
				opt_free(o);
				poptFreeContext(context);
				exit(0);
		}
	}

	/* copy list of filename / token arguments */
	extra_args = poptGetArgs(context);

	o->extra_args = NULL;
	if(extra_args != NULL) {
		i = 0;
		while(extra_args[i] != NULL) {
			o->extra_args = realloc(o->extra_args, (i+2) * sizeof(char*));
			o->extra_args[i] = malloc( (strlen(extra_args[i])+1) * sizeof(char) );
			strcpy(o->extra_args[i], extra_args[i]);
			i++;
		}
		o->extra_args[i] = NULL;
	} else {
		o->extra_args = NULL;
	}

	poptFreeContext(context);

	return o;
}

void opt_free(opt_t *o)
{
	int i;

	if(o->list_string != NULL)
		free(o->list_string);
	
	if(o->output != NULL)
		free(o->output);
	
	if(o->name != NULL)
		free(o->name);

	if(o->extra_args != NULL) {
		i = 0;
		while(o->extra_args[i] != NULL) {
			free(o->extra_args[i]);
			i++;
		}

		free(o->extra_args);
	}

	free(o);
}
