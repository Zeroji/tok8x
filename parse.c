#include "parse.h"

/* redo this, including AFTER tokenising the main file, so that
 * line numbers actually make sense. make file paths relative to
 * the main source file, too, rather than the program, and use
 * "s to surround the name so that file names with spaces are
 * understood */

t_node* parse(int set, buffer *b, int strip_cruft, int ignore_errors) {
	t_node *t;
	
	if(!b)
		return NULL;
	b=define(b);
	
	t=tokenise(set, b, strip_cruft, ignore_errors);
	
	t=conditional(t);
	t=include(t);
	
	return t;
}

buffer* define(buffer *b) {
	if(!b)
		return NULL;
	return b;
}

t_node* include(t_node *t) {
	if(!t)
		return NULL;
	return t;
}

t_node* conditional(t_node *t) {
	if(!t)
		return NULL;
	return t;
}
