//
//  CImage.c
//  FrameBuffer
//
//  Created by Alexandre Arsenault on 2016-01-08.
//  Copyright © 2016 ax. All rights reserved.
//

#include "CImage.h"
#include <stdlib.h>

CImage CreateImageFromBitmap(const char* file_path)
{
	CImage image;
	image.is_loaded = 0;

	CBitmapHeader bitmapFileHeader; // our bitmap file header

	CBitmapInfo bitmapInfoHeader;

	// Open filename in read binary mode.
	FILE* filePtr = fopen(file_path, "rb");

	if (filePtr == NULL) {
		printf("Error : Can't open image file : %s.\n", file_path);
		return image;
	}

	// read the bitmap file header
	fread(&bitmapFileHeader, sizeof(CBitmapHeader), 1, filePtr);
	
	
	printf("BSize of BMP file : %d \n", bitmapFileHeader.bfSize);
	printf("Data offset : %d \n", bitmapFileHeader.bOffBits);

	// verify that this is a bmp file by check bitmap id
	if (bitmapFileHeader.bfType != 0x4D42) {
		fclose(filePtr);
		printf("Error : Image is not a bitmap.\n");
		return image;
	}

	// Read the bitmap info header.
	fread(&bitmapInfoHeader, sizeof(CBitmapInfo), 1, filePtr);
	
	image.size.w = abs(bitmapInfoHeader.biWidth);
	image.size.h = abs(bitmapInfoHeader.biHeight);
	
	printf("Number of bye for struct : %d \n", bitmapInfoHeader.biSize);
	printf("Current struct size : %d \n", (int)sizeof(CBitmapInfo));
	printf("Number of bit per pixel : %d \n", bitmapInfoHeader.biBitCount);
	printf("Size of image in byte : %d \n", bitmapInfoHeader.biSizeImage);
	printf("Image compression : %d \n", bitmapInfoHeader.biCompression);
	printf("N pixel per meter x : %d \n", bitmapInfoHeader.biXPelsPerMeter);
	printf("N pixel per meter y : %d \n", bitmapInfoHeader.biYPelsPerMeter);
	printf("number of colors used by th ebitmap : %d \n", bitmapInfoHeader.biClrUsed);
	printf("number of colors that are important : %d \n", bitmapInfoHeader.biClrImportant);

	unsigned long data_size = bitmapInfoHeader.biSizeImage;
	
	// This is the size of the raw bitmap data, a dummy 0 can be given for BI_RGB bitmaps.
	if(data_size == 0 && bitmapInfoHeader.biCompression == 0) {
		data_size = image.size.w * image.size.h * 3;
	}
	
	if(bitmapInfoHeader.biCompression != 0) {
		fclose(filePtr);
		printf("Error : Image compression type not supported.\n");
		return image;
	}

	// Move file point to the begging of bitmap data.
	fseek(filePtr, bitmapFileHeader.bOffBits, SEEK_SET);

	// Allocate enough memory for the bitmap image data.
	image.data = (unsigned char*) malloc(data_size);

	// verify memory allocation
	if (!image.data) {
		free(image.data);
		fclose(filePtr);
		printf("Error : Can't allocate memory for image data.\n");
		return image;
	}

	// read in the bitmap image data
	fread(image.data, data_size, 1, filePtr);
	
	// make sure bitmap image data was read
	if (image.data == NULL) {
		fclose(filePtr);
		printf("Error : Can't read image data.\n");
		return image;
	}

	image.is_loaded = 1;
	
	
	fclose(filePtr);

	return image;
}