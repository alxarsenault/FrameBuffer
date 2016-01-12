//
//  CFont.h
//  FrameBuffer
//
//  Created by Alexandre Arsenault on 2016-01-08.
//  Copyright © 2016 ax. All rights reserved.
//

#ifndef CFont_h
#define CFont_h

#include <stdio.h>
#include "CRect.h"
#include "CPoint.h"
#include "CSize.h"
#include "CString.h"

typedef struct _CChar {
	CRect rect;
	CPoint delta;
	int x_next;
	int flipped;
} CChar;

typedef struct _CFont {
	CSize size;
	CChar* char_data;
	unsigned char* buffer;
} CFont;

CFont CreateFont(const char* file_path);

int GetCharIndex(CFont* font, char c);

int GetStringXSize(CFont* font, const CString* string);

#endif /* CFont_h */
