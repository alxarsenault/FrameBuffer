//
//  CFrameBuffer.h
//  FrameBuffer
//
//  Created by Alexandre Arsenault on 2016-01-08.
//  Copyright © 2016 ax. All rights reserved.
//

#ifndef CFrameBuffer_h
#define CFrameBuffer_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CColor.h"
#include "CRect.h"
#include "CPoint.h"
#include "CFont.h"
#include "CImage.h"
#include "CString.h"

typedef struct _CFrameBuffer {
	int w, h;
	CColor* data;
} CFrameBuffer;

CFrameBuffer CreateFrameBuffer(int width, int height);

void SetPixelColor(CFrameBuffer* fb, int y, int x, CColor* color);

void DrawLine(CFrameBuffer* fb, CPoint* p0, CPoint* p1, CColor* color);

void DrawRectangle(CFrameBuffer* fb, CRect* rect, CColor* color);

void DrawRectangleContour(CFrameBuffer* fb, CRect* rect, CColor* color);

int DrawChar(CFrameBuffer* fb, CFont* font, char c, CPoint* pos, CColor* color);

void DrawString(CFrameBuffer* fb, CFont* font, const char* str, CPoint* pos, CColor* color);

void DrawImage(CFrameBuffer* fb, CImage* image, CPoint* pos);

void DrawImageResize(CFrameBuffer* fb, CImage* image, CPoint* pos, CSize* size);

#endif /* CFrameBuffer_h */
