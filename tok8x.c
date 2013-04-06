/*
 * tok8x.c
 * 
 * Copyright 2013 shmibs <shmibs@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include "tok8x.h"

int main(int argc, char **argv) {	
	uint32_t i, j, bad_arg;
	uint32_t if_size;
	header h;
	struct header *h_point=&h;
	FILE *i_file;
	char *i_buffer;
	//~ FILE *o_file;
	
	/* input argument vars */
	char *a_ifilename=NULL;
	char *a_ofilename=NULL;
	int a_ignore_comments=0;
	int a_ignore_errors=0;
	t_set a_t_set=BASIC;
	
	char *help_message="\noptions:\n -h\n   show this help dialogue\n\n -s <axe|basic|grammer>\n   define token set to be used\n\n -o <filename>\n   define file to be written (defaults to out.[txt|8xp])\n\n -i\n   ignore \"comments\" (lines beginning with a .)\n\n -f\n   ignore (skip over) strings that cannot be tokenised\n";
	char *usage_message="usage: %s <filename> [options]\n";
	
/* ----------------------[ INPUT PARSING ]---------------------- */
	
	i=1;
	while(argv[i]){
		bad_arg=1;
		if(strncmp(argv[i], "-", 1)) {
			if(a_ifilename)
				break;
			a_ifilename=argv[i];
			bad_arg=0;
			
		}
		
		if( !(strncmp(argv[i], "-h", 2) && strncmp(argv[i], "--help", 6) )) {
			printf(usage_message, argv[0]);
			puts(help_message);
			return 0;
		}
		
		if( !(strncmp(argv[i], "-i", 2) )) {
			a_ignore_comments=1;
			bad_arg=0;
		}
		
		if( !(strncmp(argv[i], "-f", 2) )) {
			a_ignore_errors=1;
			bad_arg=0;
		}
		
		if( !(strncmp(argv[i], "-s", 2) )) {
			if(argv[i+1]) {
				i++;
				/* convert it to lowercase, so things like "Axe" will be recognised */
				for(j=0; argv[i][j]; j++)
					argv[i][j]=tolower(argv[i][j]);
				if( !(strncmp(argv[i], "basic", 3) )) {
					bad_arg=0;
				}
				if( !(strncmp(argv[i], "axe", 3) )) {
					a_t_set=AXE;
					bad_arg=0;
				}
				if( !(strncmp(argv[i], "grammer", 3) )) {
					a_t_set=GRAMMER;
					bad_arg=0;
				}
			}
		}
		
		if( !(strncmp(argv[i], "-o", 2) )) {
			if(argv[i+1]) {
				i++;
				if(strncmp(argv[i], "-", 1)) {
					a_ofilename=argv[i];
					bad_arg=0;
				}
			}
		}
		
		if(bad_arg)
			break;
		i++;
	}
	
	if(bad_arg) {
		puts("err: bad option");
		return 1;
	}
	
	if(!a_ifilename) {
		printf(usage_message, argv[0]);
		return 1;
	}
	
/* ----------------------[ FILE READING ]---------------------- */
	
	i_file=fopen(a_ifilename, "r");
	if(!i_file) {
		printf("err: could not read \"%s\"\n", a_ifilename);
		return 1;
	}
	
	fseek(i_file, 0, SEEK_END);
	if_size=ftell(i_file)-1;
	rewind(i_file);
	
	if(if_size>0x800000) {
		printf("err: %s is obcenely large\n", a_ifilename);
		fclose(i_file);
		return 1;
	}
	
	
	i_buffer=(char*)malloc(if_size);
	fread(i_buffer, 1, if_size, i_file);
	
	fclose(i_file);
	
	/* testing */
	token test_token;
	for(i=0; i<if_size; i++) {
		memcpy(&test_token, t_match(a_t_set, i_buffer, if_size, i), sizeof(token));
		puts(test_token.name);
	}
	
	
	//~ var_init(h_point);
	//~ header_init(h_point);
	
	//~ if(a_ofilename==NULL) {
		/* simply print the file's contents here */
	//~ } else {
		/* open a file and write to it */
	
		//~ o_file=fopen("out.8xp", "w");
		
		//~ /* write var to file */
		//~ fwrite(h_point->top, 1, 11, o_file);
		//~ fwrite(h_point->comment, 1, 42, o_file);
		//~ fwrite(&(h_point->length), 1, 2, o_file);
			//~ /* writing data section here */
			//~ fwrite(&(h_point->var.top), 1, 2, o_file);
			//~ fwrite(&(h_point->var.length), 1, 2, o_file);
			//~ fwrite(&(h_point->var.type), 1, 1, o_file);
			//~ fwrite(&(h_point->var.name), 1, 9, o_file);
			//~ fwrite(&(h_point->var.archived), 1, 1, o_file);
			//~ fwrite(&(h_point->var.length2), 1, 2, o_file);
			//~ fwrite(h_point->var.data, 1, h_point->var.length, o_file);
		//~ fwrite(&(h_point->checksum), 1, 2, o_file);
			
		//~ fclose(o_file);
	//~ }
	
	free(i_buffer);

	return 0;
}

void var_init(header *h) {
	h->var.top=0x0B;
	h->var.length=0x0B; /* manually assign length for now. this will always be the header's length entry - 0x11 */
	h->var.type=0x05; /* manually assign to program type */
	static char new_name[9]={0x4E, 0x41, 0x4D, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00}; /* manually assigned to "NAME" */
	memcpy(h->var.name, new_name, sizeof(new_name));
	h->var.archived=0x00; /* our variable will not be archived */
	h->var.length2=h->var.length;
	static uint8_t new_data[11]={0x09, 0x00, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39}; /* manually assign var data for now */
	h->var.data=&new_data[0];
}

void header_init(header *h) {
	uint16_t i;
	uint8_t *p;
	
	static uint8_t new_top[11]={0x2A, 0x2A, 0x54, 0x49, 0x38, 0x33, 0x46, 0x2A, 0x1A, 0x0A, 0x00};
	memcpy(h->top, new_top, sizeof(new_top));
	static char new_comment[42]="made with tok8x                           ";
	memcpy(h->comment, new_comment, sizeof(new_comment));
	h->length=0x1C; /* manually assign length for now */
	
	/* the checksum is calculated by adding up all of the var's contents and chopping off the upper two bytes */ 
	p=(uint8_t*)&(h->var);
	h->checksum=0;
	for(i=0; i<17; i++) {
		h->checksum+=p[i];
	}
		
	p=h->var.data;
	for(i=0; i<h->var.length; i++) {
		h->checksum+=p[i];
	}
	
}
