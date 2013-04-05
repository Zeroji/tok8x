#include "tokens.h"

/* passing back a token here will make fetching the proper return value a
 * simple matter of return t_lists[set][i]. after that, increase the cursor
 * position in the tokeniser by length of returned.name, or, if the returned
 * token had both a first and second byte value of NONE (0xFF 0xFF, which
 * does not exist in the 83+ token set), puts(returned.name) and exit
 * gracefully (reporting the line where the error occured) */
token t_match(int set, char *buffer[], const int buffer_size, const int cursor) {
	token t_return_err={NONE, NONE, "err: token not found"};
	
	
	/* match a token here */
	/* find the shortest complete token match for a substring in buffer which
	 * begins at the current cursor position */
	
	
	/* if no match exists for char at cursor, or if reaching the end of file before
	 * a complete match is found (i.e. we find something like Diagnosti, and
	 * DiagnosticOff exists as a token, so the possibility of matching has not
	 * yet been rejected, but the end of the file is reached before a match is
	 * made) */
	return t_return_err;
}
