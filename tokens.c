#include "tok8x.h"

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
