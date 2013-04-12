#include "preprocessor.h"

buffer include(buffer b) {
	FILE *f;
	char *swapbuff;
	uint32_t i, j, fsize;
	string s;
	
	/* because 11 is the minimum length of the ##include statement */
	for(i=0; i<b.size-11; i++) {
		if( !strncmp(&(b.dat[i]), "##include", 9) ) {
			/* set a marker at the beginning of the ##include, so
			 * we can successfully strip it out later */
			j=i;
			i+=9;
			s=sstrlen(b, i);
			if(s.length == 0){
				fprintf(stderr, "err: incomplete include statement\n");
				free(b.dat);
				return b;
			}
			s.val=malloc(s.length);
			if(s.val == NULL) {
				fprintf(stderr, "err: could not allocate memory\n");
				free(b.dat);
				b.dat=NULL;
				return b;
			}
			sscanf(&(b.dat[i]), "%s", s.val);
			
			f=fopen(s.val, "r");
			if(!f) {
				fprintf(stderr, "err: could not include \"%s\"\n", s.val);
				free(b.dat);
				b.dat=NULL;
				return b;
			}
			
			fseek(f, 0, SEEK_END);
			fsize=ftell(f)-1;
			rewind(f);
			
			if(fsize+b.size>0x800000) {
				fprintf(stderr, "err: project too large\n");
				fclose(f);
				return b;
			}
			free(s.val);
			
			swapbuff=malloc(b.size+fsize);
			memcpy(swapbuff, b.dat, j);
			fread(&(swapbuff[j]), fsize, 1, f);
			memcpy(&(swapbuff[j+fsize]), &(b.dat[s.end]), b.size-s.end);
			free(b.dat);
			b.dat=swapbuff;
			b.size=b.size+fsize-(s.end-j);
			fclose(f);
		}
	}
	
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
