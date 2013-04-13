#include "preprocessor.h"

/* redo this, including AFTER tokenising the main file, so that
 * line numbers actually make sense. make file paths relative to
 * the main source file, too, rather than the program, and use
 * "s to surround the name so that file names with spaces are
 * understood */

t_node* include(t_node* b) {
	return b;
}

/* get the length and position of the next string that would be grabbed by sscanf */
string sstrlen(buffer b, uint32_t cursor) {
	string s;
	s.length=0;
	s.end=cursor;
	
	/* head first to the next non-whitespace, or return 0 if none is found */
	while( !strncmp(&(b.dat[cursor]), " ", 1) || !strncmp(&(b.dat[cursor]), "\n", 1) || !strncmp(&(b.dat[cursor]), "\t", 1) ) {
		cursor++;
		if(cursor == b.size)
			return s;
	}
	s.start=cursor;
	
	while( strncmp(&(b.dat[cursor]), " ", 1) && strncmp(&(b.dat[cursor]), "\n", 1) && strncmp(&(b.dat[cursor]), "\t", 1) ) {
		s.length++;
		cursor++;
	}
	s.end=cursor;
	
	return s;
}
