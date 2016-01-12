//
//  CPoint.c
//  FrameBuffer
//
//  Created by Alexandre Arsenault on 2016-01-08.
//  Copyright © 2016 ax. All rights reserved.
//

#include "CPoint.h"

CPoint CPoint_Create(int x, int y)
{
	CPoint pt;
	pt.x = x;
	pt.y = y;
	return pt;
}