//
//  CColor.h
//  FrameBuffer
//
//  Created by Alexandre Arsenault on 2016-01-08.
//  Copyright © 2016 ax. All rights reserved.
//

#ifndef CColor_h
#define CColor_h

typedef struct _CColor {
//	unsigned char r, g, b;
	unsigned char b, g, r, a;
} CColor;

CColor CreateColor(int r, int g, int b);

#endif /* CColor_h */
