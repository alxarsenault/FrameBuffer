//
//  CSize.h
//  FrameBuffer
//
//  Created by Alexandre Arsenault on 2016-01-08.
//  Copyright © 2016 ax. All rights reserved.
//

#ifndef CSize_h
#define CSize_h

typedef struct _CSize {
	int w, h;
} CSize;

CSize CreateSize(int w, int h);

#endif /* CSize_h */
