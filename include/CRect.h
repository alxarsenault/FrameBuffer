//
//  CRect.h
//  FrameBuffer
//
//  Created by Alexandre Arsenault on 2016-01-08.
//  Copyright © 2016 ax. All rights reserved.
//

#ifndef CRect_h
#define CRect_h

typedef struct _CRect {
	int x, y, w, h;
} CRect;

CRect CreateRect(int x, int y, int w, int h);

#endif /* CRect_h */
