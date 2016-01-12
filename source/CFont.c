//
//  CFont.c
//  FrameBuffer
//
//  Created by Alexandre Arsenault on 2016-01-08.
//  Copyright © 2016 ax. All rights reserved.
//

#include "CFont.h"
#include <stdlib.h>

const char* all_chars = " !\"#$%&'()*+,-./0123456789:;<=>?"
						"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
						"`abcdefghijklmnopqrstuvwxyz{|}~";
const int N_CHAR = 95;

CFont CreateFont(const char* file_path)
{
	CFont font;

	FILE* font_file = fopen(file_path, "rb");

	if (font_file == NULL) {
		printf("Error : Unable to open font file : %s.", file_path);
		return font;
	}

	fread(&font.size, sizeof(CSize), 1, font_file);

	font.char_data = malloc(N_CHAR * sizeof(CChar));
	fread(font.char_data, sizeof(CChar), N_CHAR, font_file);

	const unsigned long buffer_size = font.size.w * font.size.h * sizeof(unsigned char);
	font.buffer = malloc(buffer_size);
	fread(font.buffer, buffer_size, 1, font_file);

	fclose(font_file);

	return font;
}

int GetCharIndex(CFont* font, char c)
{
	for(int i = 0; i < N_CHAR; i++) {
		if(c == all_chars[i]) {
			return i;
		}
	}
	return -1;
}

int GetStringXSize(CFont* font, const CString* string)
{
	int str_length = CString_GetSize(string);
	int total_x = 0;
	
	for (int i = 0; i < str_length; i++) {
		total_x += font->char_data[GetCharIndex(font, string->data[i])].x_next;
	}
	
	return total_x;
}
