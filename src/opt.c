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
			.arg = &(o->token_set),
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
				break;

			case OPT_INFO:
				o->info = true;
				if(o->output || o->name || o->archived || o->strip) {
					printf("\e[1mtok8x:\e[0m"
							"\e[1;31merror:\e[0m %s\n",
							"conflicting arguments"
							);
				}
				break;

			case OPT_OUTPUT:
				if(o->info) {
					printf("\e[1mtok8x:\e[0m"
							"\e[1;31merror:\e[0m %s\n",
							"conflicting arguments"
							);
				}
				break;

			case OPT_NAME:
				if(o->info) {
					printf("\e[1mtok8x:\e[0m"
							"\e[1;31merror:\e[0m %s\n",
							"conflicting arguments"
							);
				}
				break;

			case OPT_ARCHIVED:
				if(o->info) {
					printf("\e[1mtok8x:\e[0m"
							"\e[1;31merror:\e[0m %s\n",
							"conflicting arguments"
							);
				}
				o->archived = true;
				break;

			case OPT_PRETTY:
				o->pretty = true;
				break;

			case OPT_STRIP:
				if(o->info) {
					printf("\e[1mtok8x:\e[0m"
							"\e[1;31merror:\e[0m %s\n",
							"conflicting arguments"
							);
				}
				o->strip = true;
				break;

			default:
				if(r != OPT_HELP) {
					printf("\e[1mtok8x:\e[0m"
							"\e[1;31merror:\e[0m \e[1m%s\e[0m: %s\n",
							poptBadOption(context, 0),
							poptStrerror(r)
							);
				} else {
					poptPrintHelp(context, stdout, 0);
				}

				poptFreeContext(context);

				if(r == OPT_HELP)
					exit(0);

				opt_free(o);

				exit(EINVAL);
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

	if(o->token_set != NULL)
		free(o->token_set);
	
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
