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

#include <stdio.h>
#include "tok8x.h"

int main(int argc, char **argv) {	
	header h;
	struct header *h_point=&h;
	
	FILE *of;
	
	var_init(h_point);
	header_init(h_point);
	
	of=fopen("out.8xp", "w");
	
	/* write var to file */
	fwrite(h_point->top, 1, 11, of);
	fwrite(h_point->comment, 1, 42, of);
	fwrite(&(h_point->length), 1, 2, of);
		fwrite(&(h_point->var.top), 1, 2, of);
		fwrite(&(h_point->var.length), 1, 2, of);
		fwrite(&(h_point->var.type), 1, 1, of);
		fwrite(&(h_point->var.name), 1, 9, of);
		fwrite(&(h_point->var.archived), 1, 1, of);
		fwrite(&(h_point->var.length2), 1, 2, of);
		fwrite(h_point->var.data, 1, h_point->var.length, of);
	fwrite(&(h_point->checksum), 1, 2, of);
		
	fclose(of);
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
	//~ uint16_t i;
	//~ uint8_t *p;
	
	static uint8_t new_top[11]={0x2A, 0x2A, 0x54, 0x49, 0x38, 0x33, 0x46, 0x2A, 0x1A, 0x0A, 0x00};
	memcpy(h->top, new_top, sizeof(new_top));
	static char new_comment[42]="made with tok8x                           ";
	memcpy(h->comment, new_comment, sizeof(new_comment));
	h->length=0x1C; /* manually assign length for now */
	
	//~ /* the checksum is calculated by adding up all of the var's contents and chopping off the upper two bytes */ 
	//~ p=(uint8_t)&(h->var);
	//~ h->checksum=0;
	//~ for(i=0; i<17; i++) {
		//~ h->checksum+=(uint8_t)*(p+i);
	//~ }
	//~ p=h->var.data;
	//~ for(i=0; i<17; i++) {
		//~ h->checksum+=(uint8_t)*(p+i);
	//~ }

	/* set manually for now until i can get the above to work properly */
	h->checksum=0x032D;
	
}
