#include "opt.h"

enum {
	OPT_HELP = 1,
	OPT_TOKEN_SET = 2,
	OPT_INPUT = 3,
	OPT_OUTPUT = 4,
	OPT_NAME = 5,
	OPT_ARCHIVED = 6,
	OPT_PRETTY = 7,
	OPT_STRIP = 8,
};

opt_t* opt_read(int argc, const char *argv[])
{
	int r;
	opt_t *o = calloc(1, sizeof(opt_t));

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
			.longName = "token_set",
			.shortName = 't',
			.argInfo = POPT_ARG_STRING,
			.arg = o->token_set,
			.val = OPT_TOKEN_SET,
			.descrip = "define token set to be used",
			.argDescrip = "<axe|basic|grammer>"
		},
		{
			.longName = "input",
			.shortName = 'i',
			.argInfo = POPT_ARG_STRING,
			.arg = o->input,
			.val = OPT_INPUT,
			.descrip = "define file to be read (defaults to stdin)",
			.argDescrip = "<filename>"
		},
		{
			.longName = "output",
			.shortName = 'o',
			.argInfo = POPT_ARG_STRING,
			.arg = o->output,
			.val = OPT_OUTPUT,
			.descrip = "define file to be written (defaults to stdout)",
			.argDescrip = "<filename>"
		},
		{
			.longName = "name",
			.shortName = 'n',
			.argInfo = POPT_ARG_STRING,
			.arg = o->name,
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
			.descrip = "generate archived program. valid only with 8xp output",
			.argDescrip = NULL
		},
		{
			.longName = "pretty",
			.shortName = 'p',
			.argInfo = POPT_ARG_NONE,
			.arg = NULL,
			.val = OPT_PRETTY,
			.descrip = "use \"pretty\" output, i.e. unicode approximations "\
						"that more closely resemble the true appearances. "\
						"valid only with 8xp input",
			.argDescrip = NULL
		},
		{
			.longName = "strip",
			.shortName = 's',
			.argInfo = POPT_ARG_NONE,
			.arg = NULL,
			.val = OPT_STRIP,
			.descrip = "strip excess data (comments, spaces, empty lines, "\
						"etc). valid only with 8xp output",
			.argDescrip = NULL
		},
		{ NULL, 0, 0, NULL, 0 }
	};

	poptContext context = poptGetContext("tok8x", argc, argv, options, 0);

	while( (r = poptGetNextOpt(context)) != -1) {
		switch(r) {
			case OPT_TOKEN_SET:
				break;

			case OPT_INPUT:
				break;

			case OPT_OUTPUT:
				break;

			case OPT_NAME:
				break;

			case OPT_ARCHIVED:
				break;

			case OPT_PRETTY:
				break;

			case OPT_STRIP:
				break;

			default:
				if(r != OPT_HELP) {
					printf("\e[1;31merr:\e[0m \e[1m%s\e[0m: %s\n\n",
							poptBadOption(context, 0),
							poptStrerror(r)
							);
					poptPrintHelp(context, stderr, 0);
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

	poptFreeContext(context);

	return o;
}

void opt_free(opt_t *o)
{
	if(o->token_set != NULL)
		free(o->token_set);
	
	if(o->input != NULL)
		free(o->input);
	
	if(o->output != NULL)
		free(o->output);
	
	if(o->name != NULL)
		free(o->name);

	free(o);
}
