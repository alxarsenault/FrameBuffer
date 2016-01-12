//
//  CRect.c
//  FrameBuffer
//
//  Created by Alexandre Arsenault on 2016-01-08.
//  Copyright © 2016 ax. All rights reserved.
//

#include "CRect.h"

CRect CreateRect(int x, int y, int w, int h)
{
	CRect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	
	return rect;
}