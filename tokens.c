#include "tokens.h"

token t_axe[] = {
	{ 0x29, NONE, " " },
	{ 0x3E, NONE, ":" },
	{ 0x2B, NONE, "." },
	{ 0x04, NONE, "->" },
	{ 0x06, NONE, "[" },
	{ 0x07, NONE, "]" },
	{ 0x08, NONE, "{" },
	{ 0x09, NONE, "}" },
	{ 0x0A, NONE, "^^r" },
	{ 0x0B, NONE, "^^o" },
	{ 0x0C, NONE, "^^-1" },
	{ 0xBB, 0x66, "DiagnosticOn" },
	{ 0xBB, 0x67, "DiagnosticOff" },
	{ 0x75, NONE, "Full" },
	{ 0x66, NONE, "Normal" },
	{ 0xD8, NONE, "Pause" },
	{ 0xAD, NONE, "getKey" }
};

token t_basic[] = {
};

token t_grammer[] = {
};

token *t_lists[] = {
	t_axe,
	t_basic,
	t_grammer
};

int t_list_lengths[] = {
	(int)(sizeof(t_axe)/sizeof(token)),
	(int)(sizeof(t_basic)/sizeof(token)),
	(int)(sizeof(t_grammer)/sizeof(token))
};

/* list of values for determining, when converting
 * from tokens to plaintext, if the second byte
 * should be considered */
uint8_t t_2byte_indicators[] = {
	/* Matrices */
	0x5C,
	/* Lists */
	0x5D,
	/* Equations */
	0x5E,
	/* Pictures */
	0x60,
	/* GDBs */
	0x61,
	/* Strings */
	0xAA,
	/* Statistics */
	0x62,
	/* Window and Finance */
	0x63,
	/* Graph Format */
	0x7E,
	/* Miscellaneous */
	0xBB,
	/* 84+ only */
	0xEF
};
