#include "tok8x.h"
#include "tokens.h"

/* passing back a token here will make fetching the proper return value a
 * simple matter of return t_lists[set][i]. after that, increase the cursor
 * position in the tokeniser by sizeof(returned.name), or, if the returned
 * token had both a first and second byte value of NONE (0xFF 0xFF, which
 * does not exist in the 83+ token set), puts(returned.name) and exit
 * gracefully (reporting the line where the error occured) */
token t_match(int set, char *buffer[], const int buffer_size, const int cursor) {
	token t_return_err={NONE, NONE, "err: token not found"};
	
	/* match tokens here */
	
	return t_return_err;
}
