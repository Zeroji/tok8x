/*
 * tok8x.h
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
#ifndef _TOK8X

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define NONE 0xFF

/* struct for storing tokens in an lut. for single-byte
 * tokens, b_second will equal NONE|0xFF */
typedef struct token {
	uint8_t b_first;
	uint8_t b_second;
	const char name[20];
} token;

/* struct for storing tokens in linked list. for single-byte
 * tokens, b_second will equal NONE|0xFF */
typedef struct t_node {
	uint8_t b_first;
	uint8_t b_second;
	struct t_node *next;
} t_node;

/* will contain data for program vars */
typedef struct variable {
	uint16_t top;
	uint16_t length;
	uint8_t type;
	uint8_t name[9]; /* only 8 bytes allowed. last must be 0 */
	uint8_t archived; /* 0x00 if unarchived, 0x80 if archived */
	uint16_t length2; /* a duplicate of length */
	uint8_t *data;
} variable;

typedef struct header {
	uint8_t top[11];
	uint8_t comment[42];
	uint16_t length;
	variable var;
	uint32_t checksum;
} header;

extern void header_init(header *p);
extern void var_init(header *p);

extern token* tokenise(char *buffer);

/* this one will be stuck over int tokens.h to
 * make it private later, after i'm done testing */
token t_match(int set, char *buffer[], const int buffer_size, const int cursor);


#endif
