#include "preprocessor.h"

/* redo this, including AFTER tokenising the main file, so that
 * line numbers actually make sense. make file paths relative to
 * the main source file, too, rather than the program, and use
 * "s to surround the name so that file names with spaces are
 * understood */

t_node* preprocess(t_node* b) {
	if(!b)
		return NULL;
	return b;
}

t_node* define(t_node* b) {
	if(!b)
		return NULL;
	return b;
}

t_node* include(t_node* b) {
	if(!b)
		return NULL;
	return b;
}

t_node* conditional(t_node* b) {
	if(!b)
		return NULL;
	return b;
}
