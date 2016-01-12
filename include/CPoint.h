//
//  CPoint.h
//  FrameBuffer
//
//  Created by Alexandre Arsenault on 2016-01-08.
//  Copyright © 2016 ax. All rights reserved.
//

#ifndef CPoint_h
#define CPoint_h

typedef struct _CPoint {
	int x, y;
} CPoint;

//------------------------------------------------------------------------------
// Constructor.
//------------------------------------------------------------------------------
CPoint CPoint_Create(int x, int y);

#endif /* CPoint_h */
