//
//  CSize.c
//  FrameBuffer
//
//  Created by Alexandre Arsenault on 2016-01-08.
//  Copyright © 2016 ax. All rights reserved.
//

#include "CSize.h"

CSize CreateSize(int w, int h)
{
	CSize size;
	size.w = w;
	size.h = h;
	return size;
}