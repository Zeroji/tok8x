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
	FILE *o_file;
	t_node *o_buffer;
	t_node *traverse;
	
	/* input argument vars */
	char *a_ifilename=NULL;
	char *a_ofilename=NULL;
	int a_ignore_comments=0;
	int a_ignore_errors=1;
	uint8_t a_archived=0x00;
	t_set a_t_set=BASIC;
	char a_internal_name[9]={0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	char *help_message="\noptions:\n -h\n   show this help dialogue\n\n -t <axe|basic|grammer>\n   define token set to be used\n\n -o <filename>\n   define file to be written (defaults to out.[txt|8xp])\n\n -n <name>\n   define on-calc name (warning: does not check name validity)\n\n -a\n   generate archived program\n\n -i\n   ignore \"comments\" (lines beginning with a .)\n\n -s\n   use strict mode (do not ignore unparseable tokens)\n";
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
		
		if( !(strncmp(argv[i], "-s", 2) )) {
			a_ignore_errors=0;
			bad_arg=0;
		}
		
		if( !(strncmp(argv[i], "-a", 2) )) {
			a_archived=0x80;
			bad_arg=0;
		}
		
		if( !(strncmp(argv[i], "-t", 2) )) {
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
		
		if( !(strncmp(argv[i], "-n", 2) )) {
			if(argv[i+1]) {
				i++;
				if(strncmp(argv[i], "-", 1)) {
					if(strlen(argv[i]) > 8)
						argv[i][8]=0;
					strcpy(&(a_internal_name[0]), argv[i]);
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
		printf("err: bad input file \"%s\"\n", a_ifilename);
		fclose(i_file);
		return 1;
	}
	
	
	i_buffer=(char*)malloc(if_size);
	fread(i_buffer, 1, if_size, i_file);
	
	fclose(i_file);

/* ----------------------[ FILE PARSING ]---------------------- */
	
	if( !strncmp(i_buffer, "**TI83F*", 8) ) {
		puts("this is an 8X program!");
		/* do detokenising stuff here! */
		return 0;
	} else {
		o_buffer=tokenise(a_t_set, i_buffer, if_size, a_ignore_comments, a_ignore_errors);
		if( !o_buffer ) {
			free(i_buffer);
			return 1;
		}
	}
	
	
/* ----------------------[ FILE WRITING ]---------------------- */
	
	
	var_init(h_point, o_buffer, &(a_internal_name[0]), a_archived);
	header_init(h_point, o_buffer);

	if(a_ofilename == NULL) {
		/* simply print the file's contents here */
	} else {
		/* open a file and write to it */
		o_file=fopen("out.8xp", "w");

		/* write var to file */
		fwrite(h_point->top, 1, 11, o_file);
		fwrite(h_point->comment, 1, 42, o_file);
		fwrite(&(h_point->length), 1, 2, o_file);
			/* writing data section here */
			fwrite(&(h_point->var.top), 1, 2, o_file);
			fwrite(&(h_point->var.length), 1, 2, o_file);
			fwrite(&(h_point->var.type), 1, 1, o_file);
			fwrite(&(h_point->var.name), 1, 9, o_file);
			fwrite(&(h_point->var.archived), 1, 1, o_file);
			fwrite(&(h_point->var.length2), 1, 2, o_file);
			fwrite(&(h_point->var.length3), 1, 2, o_file);
				/* write data contents here */
				traverse=o_buffer;
				while(traverse) {
					fwrite(&(traverse->b_first), 1, 1, o_file);
					if(traverse->b_second != NONE)
						fwrite(&(traverse->b_second), 1, 1, o_file);
					traverse=traverse->next;
				}
		fwrite(&(h_point->checksum), 1, 2, o_file);
			
		fclose(o_file);
	}
	
	free_list(o_buffer);
	free(i_buffer);

	return 0;
}

void var_init(header *h, t_node *list_head, char *a_name, uint8_t a_archived) {
	h->var.top=0x0D;
	h->var.length=get_list_length(list_head)+0x02;
	h->var.type=0x05; /* manually assign to program type */
	//~ static char new_name[9]={0x4E, 0x41, 0x4D, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00}; /* manually assigned to "NAME" */
	//~ memcpy(h->var.name, new_name, sizeof(new_name));
	strcpy((char*)h->var.name, a_name);
	h->var.archived=a_archived;
	h->var.length2=h->var.length;
	h->var.length3=h->var.length-0x02;
}

void header_init(header *h, t_node *list_head) {
	uint16_t i;
	uint8_t *p;
	
	static uint8_t new_top[11]={0x2A, 0x2A, 0x54, 0x49, 0x38, 0x33, 0x46, 0x2A, 0x1A, 0x0A, 0x00};
	memcpy(h->top, new_top, sizeof(new_top));
	static char new_comment[42]="made with tok8x                           ";
	memcpy(h->comment, new_comment, sizeof(new_comment));
	h->length=get_list_length(list_head)+0x13;
	
	/* the checksum is calculated by adding up all of the var's contents and chopping off the upper two bytes */ 
	p=(uint8_t*)&(h->var);
	h->checksum=0;
	for(i=0; i<19; i++) {
		h->checksum+=p[i];
	}
	
	t_node *traverse=list_head;
	while(traverse) {	
		h->checksum+=traverse->b_first;
		if(traverse->b_second != NONE)
			h->checksum+=traverse->b_second;
		traverse=traverse->next;
	}
	
}

void free_list(t_node *list_head) {
	t_node *temp;
	while(list_head) {
		temp=list_head;
		list_head=list_head->next;
		free(temp);
	}
}

uint16_t get_list_length(t_node *list_head) {
	uint16_t r_length=0;
	while(list_head) {
		if(list_head->b_second != NONE)
			r_length++;
		list_head=list_head->next;
		r_length++;
	}
	return r_length;
}
