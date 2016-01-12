//
//  CImage.h
//  FrameBuffer
//
//  Created by Alexandre Arsenault on 2016-01-08.
//  Copyright © 2016 ax. All rights reserved.
//

#ifndef CImage_h
#define CImage_h

#include <stdio.h>
#include <stdint.h>
#include "CSize.h"

#pragma pack(1)

typedef struct _CBitmapHeader
{
	uint16_t bfType;  //specifies the file type
	uint32_t bfSize;  //specifies the size in bytes of the bitmap file
	uint16_t bfReserved1;  //reserved; must be 0
	uint16_t bfReserved2;  //reserved; must be 0
	uint32_t bOffBits;  // species the offset in bytes from the bitmapfileheader to the bitmap bits
	// The offset, i.e. starting address, of the byte where the bitmap image data (pixel array) can be found.
} __attribute__((packed)) CBitmapHeader;

//typedef struct
//{
//	unsigned int size;
//	int width,height;
//	unsigned short int planes;
//	unsigned short int bpp;
//	unsigned int compression;
//	unsigned int imagesize;
//	int xresolution,yresolution;
//	unsigned int colours;
//	unsigned int impcolours;
//}INFOHEADER;

typedef struct _CBitmapInfo
{
	uint32_t biSize;  //specifies the number of bytes required by the struct
	int32_t biWidth;  //specifies width in pixels
	int32_t biHeight;  //species height in pixels
	uint16_t biPlanes; //specifies the number of color planes, must be 1
	uint16_t biBitCount; //specifies the number of bit per pixel
	uint32_t biCompression;//spcifies the type of compression
	uint32_t biSizeImage;  //size of image in bytes
	int32_t biXPelsPerMeter;  //number of pixels per meter in x axis
	int32_t biYPelsPerMeter;  //number of pixels per meter in y axis
	uint32_t biClrUsed;  //number of colors used by th ebitmap
	uint32_t biClrImportant;  //number of colors that are important
} __attribute__((packed)) CBitmapInfo;

#pragma pack()

typedef struct _CImage {
	int is_loaded;
	CSize size;
	unsigned char* data;
} CImage;

CImage CreateImageFromBitmap(const char* file_path);

#endif /* CImage_h */
