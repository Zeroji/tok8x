#ifndef	ERR_H
#define	ERR_H

#include <stdlib.h>
#include <stdio.h>

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
			exit(1); \
		} \
	} while(0)

#define EXIT_FERROR(file) \
	do { \
		if(ferror(file)) { \
			PRINT_EXIT_CONTEXT(); \
			exit(1); \
		} \
	} while(0)

#else /* DEBUG */

#define EXIT_NULL(val) \
	do { \
		if((val) == NULL) { \
			exit(1); \
		} \
	} while(0)

#define EXIT_FERROR(file) \
	do { \
		if(ferror(file)) { \
			exit(1); \
		} \
	} while(0)

#endif /* DEBUG */

#endif
