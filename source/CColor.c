//
//  CColor.c
//  FrameBuffer
//
//  Created by Alexandre Arsenault on 2016-01-08.
//  Copyright © 2016 ax. All rights reserved.
//

#include "CColor.h"

CColor CreateColor(int r, int g, int b)
{
	CColor c;
	c.r = r;
	c.g = g;
	c.b = b;
	return c;
}