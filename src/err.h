#ifndef	TOK8X_ERR_H
#define	TOK8X_ERR_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG /* DEBUG */

#define PRINT_EXIT_CONTEXT() \
	do { \
		printf("file: %s\n", __FILE__); \
		printf("func: %s\n", __func__); \
		printf("line: %d\n", __LINE__); \
	} while(0)

#define EXIT_NULL(val) \
	do { \
		if((val) == NULL) { \
			PRINT_EXIT_CONTEXT(); \
			exit(errno); \
		} \
	} while(0)

#define EXIT_FERROR(file) \
	do { \
		if(ferror(file)) { \
			PRINT_EXIT_CONTEXT(); \
			exit(errno); \
		} \
	} while(0)

#define EXIT_NEQUAL(a, b) \
	do { \
		if((a) != (b)) { \
			PRINT_EXIT_CONTEXT(); \
			exit(EPERM); \
		} \
	} while(0)


#else /* DEBUG */

#define EXIT_NULL(val) \
	do { \
		if((val) == NULL) { \
			exit(errno); \
		} \
	} while(0)

#define EXIT_FERROR(file) \
	do { \
		if(ferror(file)) { \
			exit(errno); \
		} \
	} while(0)

#define EXIT_NEQUAL(a, b) \
	do { \
		if((a) != (b)) { \
			exit(EPERM); \
		} \
	} while(0)

#endif /* DEBUG */

#endif
