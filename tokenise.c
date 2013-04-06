#include "tokens.h"

/* passing back a token here will make fetching the proper return value a
 * simple matter of return t_lists[set][i]. after that, increase the cursor
 * position in the tokeniser by length of returned.name, or, if the returned
 * token had both a first and second byte value of NONE (0xFF 0xFF, which
 * does not exist in the 83+ token set), puts(returned.name) and exit
 * gracefully (reporting the line where the error occured) */
token* t_match(int set, char buffer[], const int buffer_size, const int cursor) {
	token t_return={NONE, NONE, ""};
	token *rp=&t_return;
	
	/* match a token here */
	/* find the longest complete token match for a substring in buffer which
	 * begins at the current cursor position */
	int i, j, match=0, length_left=buffer_size-cursor;
	printf("----matching %c at %d----\n", buffer[cursor], cursor);
	for(i=0; i<t_list_lengths[set]; i++) {
		/* if the first character matches */
		if( !strncmp(&buffer[cursor], t_lists[set][i].name, 1) ) {
			/* if the potential match isn't too long to fit in
			 * what remains of the file */
			if( strlen(t_lists[set][i].name) <= length_left ) {				
				/* check if the two strings are actually equal */
				if( !strncmp(&buffer[cursor], t_lists[set][i].name, strlen(t_lists[set][i].name)) ) {
					/* check if the length of the found string is greater than the one already present */
					if( strlen(t_lists[set][i].name) > strlen(t_return.name) ) {
						/* we found a match! */
						match=1;
						t_return.b_first=t_lists[set][i].b_first;
						t_return.b_second=t_lists[set][i].b_second;
						strcpy(t_return.name, t_lists[set][i].name);
					}
				}
				
			}
		}
	}
	
	if(!match)
		strcpy(t_return.name, "err: token not found");
	
	/* if no match exists for char at cursor, or if reaching the end of file before
	 * a complete match is found (i.e. we find something like Diagnosti, and
	 * DiagnosticOff exists as a token, so the possibility of matching has not
	 * yet been rejected, but the end of the file is reached before a match is
	 * made) */
	return rp;
}
