//
//  CFrameBuffer.c
//  FrameBuffer
//
//  Created by Alexandre Arsenault on 2016-01-08.
//  Copyright © 2016 ax. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include "CDebug.h"
#include "CFrameBuffer.h"

CFrameBuffer CreateFrameBuffer(int width, int height)
{
	CFrameBuffer fb;
	fb.w = width;
	fb.h = height;

	const unsigned long size = width * height * sizeof(CColor);
	fb.data = malloc(size);
	memset((void*)fb.data, 0, size);

	return fb;
}

void SetPixelColor(CFrameBuffer* fb, int y, int x, CColor* color)
{
	const unsigned long index = y * fb->w + x;
	fb->data[index] = *color;
}

void DrawLine(CFrameBuffer* fb, CPoint* p0, CPoint* p1, CColor* color)
{
	// Bresenham's line algorithm.
	// http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C

	int x0 = p0->x;
	int y0 = p0->y;
	int x1 = p1->x;
	int y1 = p1->y;

	const int dx = abs(x1 - x0);
	const int sx = x0 < x1 ? 1 : -1;

	const int dy = abs(y1 - y0);
	const int sy = y0 < y1 ? 1 : -1;

	int err = (dx > dy ? dx : -dy) / 2;

	while (1) {
		SetPixelColor(fb, y0, x0, color);

		if (x0 == x1 && y0 == y1) {
			break;
		}

		if (err > -dx) {
			err -= dy;
			x0 += sx;
		}

		if (err < dy) {
			err += dx;
			y0 += sy;
		}
	}
}

void DrawRectangle(CFrameBuffer* fb, CRect* rect, CColor* color)
{
#if C_DEBUG == 1
	if (rect->x + rect->w >= fb->w) {
		printf("Error in DrawRectangle : recttangle is too wide for framebuffer.");
		return;
	}

	if (rect->y + rect->h >= fb->h) {
		printf("Error in DrawRectangle : recttangle is too high for framebuffer.");
		return;
	}
#endif

	const int iy = rect->y;
	const int ix = rect->x;

	for (int y = 0; y < rect->h; y++) {
		for (int x = 0; x < rect->w; x++) {
			SetPixelColor(fb, iy + y, ix + x, color);
		}
	}
}

void DrawRectangleContour(CFrameBuffer* fb, CRect* rect, CColor* color)
{
	CPoint tl = CPoint_Create(rect->x, rect->y);
	CPoint tr = CPoint_Create(rect->x + rect->w, rect->y);
	CPoint br = CPoint_Create(rect->x + rect->w, rect->y + rect->h);
	CPoint bl = CPoint_Create(rect->x, rect->y + rect->h);

	DrawLine(fb, &tl, &tr, color); // Top line.
	DrawLine(fb, &bl, &br, color); // Bottom line.
	DrawLine(fb, &tl, &bl, color); // Left line.
	DrawLine(fb, &tr, &br, color); // Right line.
}

int LineairInterpole(int c0, int c1, float m)
{
	return floor(c0 + (c1 - c0) * m);
}

int DrawChar(CFrameBuffer* fb, CFont* font, char c, CPoint* pos, CColor* color)
{
	int index = GetCharIndex(font, c);

	if (index == -1) {
		printf("Error : Char is not in font map.");
		return -1;
	}

	CChar* cchar = &font->char_data[index];

	const int iy = pos->y - cchar->delta.y;
	const int ix = pos->x + cchar->delta.x;

	const int cy = cchar->rect.y;
	const int cx = cchar->rect.x;

	int ch = cchar->rect.h;
	int cw = cchar->rect.w;

	for (int y = 0; y < ch; y++) {
		for (int x = 0; x < cw; x++) {

			unsigned long char_pixel_index;

			if (cchar->flipped) {
				char_pixel_index = (cy + x) * font->size.w + cx + y;
			}
			else {
				char_pixel_index = (cy + y) * font->size.w + cx + x;
			}

			int v = font->buffer[char_pixel_index];
			float c_alpha = v / 255.0f;

			const unsigned long bg_index = (iy + y) * fb->w + ix + x;

			CColor* bg_color = &fb->data[bg_index];

			int r = LineairInterpole(bg_color->r, color->r, c_alpha);
			int g = LineairInterpole(bg_color->g, color->g, c_alpha);
			int b = LineairInterpole(bg_color->b, color->b, c_alpha);

			CColor c_color = CreateColor(r, g, b);
			SetPixelColor(fb, iy + y, ix + x, &c_color);
		}
	}

	return cchar->x_next;
}

void DrawString(CFrameBuffer* fb, CFont* font, const char* str, CPoint* pos, CColor* color)
{
	int str_length = (int)strlen(str);
	CPoint char_pos = *pos;

	int next_x = pos->x;

	for (int i = 0; i < str_length; i++) {
		next_x = DrawChar(fb, font, str[i], &char_pos, color);

		if (next_x == -1) {
			printf("Error while drawing string.");
			return;
		}

		char_pos.x += next_x;
	}
}

void DrawImage(CFrameBuffer* fb, CImage* image, CPoint* pos)
{
	if (image->is_loaded == 0) {
		printf("Error : Can't draw unloaded image.");
		return;
	}

	const int iy = pos->y;
	const int ix = pos->x;

	const int img_h = image->size.h;
	const int img_w = image->size.w;

	CColor* image_pixels = (CColor*)image->data;

	for (int y = 0; y < img_h; y++) {
		for (int x = 0; x < img_w; x++) {
			unsigned long img_pixel_index = y * img_w + x;
			SetPixelColor(fb, iy + y, ix + x, &image_pixels[img_pixel_index]);
		}
	}
}

void DrawImageResize(CFrameBuffer* fb, CImage* image, CPoint* pos, CSize* size)
{
	if (image->is_loaded == 0) {
		printf("Error : Can't draw unloaded image.");
		return;
	}

	const int iy = pos->y;
	const int ix = pos->x;

	const int img_h = image->size.h;
	const int img_w = image->size.w;

	const int draw_h = size->h;
	const int draw_w = size->w;

	const float resize_step_y = (float)img_h / (float)draw_h;
	const float resize_step_x = (float)img_w / (float)draw_w;

	float y_img_index = 0.0f;
	float x_img_index = 0.0f;

	CColor* image_pixels = (CColor*)image->data;

	for (int y = 0; y < draw_h; y++) {
		for (int x = 0; x < draw_w; x++) {
			unsigned long img_pixel_index = floor(y_img_index) * img_w + floor(x_img_index);
			CColor* pixel = &image_pixels[img_pixel_index];

			SetPixelColor(fb, iy + y, ix + x, pixel);

			x_img_index += resize_step_x;

			if (x_img_index > img_w - 1) {
				x_img_index = img_w - 1;
			}
		}
		x_img_index = 0;
		y_img_index += resize_step_y;

		if (y_img_index > img_h - 1) {
			y_img_index = img_h - 1;
		}
	}
}
