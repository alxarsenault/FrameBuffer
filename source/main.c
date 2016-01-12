#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <linux/fb.h>

#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>


#include "CFrameBuffer.h"

typedef struct _eosFrameBuffer
{
	int fb_fd;
	unsigned int buffer_size;
	struct fb_fix_screeninfo fb_fix;
	struct fb_var_screeninfo fb_var;
	void* buffer;
} eosFrameBuffer;

int eosFrameBuffer_Open(eosFrameBuffer* fb)
{
	const char fb_file_path[] = "/dev/fb0";
	
	// Open framebuffer file descriptor.
	if((fb->fb_fd = open(fb_file_path, O_RDWR)) < 0) {	
		printf("Error : Can't open framebuffer file descriptor %s.\n", fb_file_path);
		return 1;
	}
	
	// Get framebuffer fix info.
	if (ioctl(fb->fb_fd, FBIOGET_FSCREENINFO, &fb->fb_fix) < 0) {
		printf("Error : Can't retrieve screen fixed info.\n");
		close(fb->fb_fd);
		return 2;
	}

	// Get framebuffer variable info.
	if (ioctl(fb->fb_fd, FBIOGET_VSCREENINFO, &fb->fb_var) < 0) {
		printf("Error : Can't retrieve screen variable info.\n");
		close(fb->fb_fd);
		return 3;
	}

	// Map framebuffer memory.	
	fb->buffer_size = fb->fb_fix.smem_len;
	fb->buffer = mmap(NULL, fb->buffer_size, PROT_READ | PROT_WRITE, 
		MAP_SHARED, fb->fb_fd, 0);

	if(fb->buffer == NULL) {
		printf("Error : Can't map framebuffer memory.\n");
		close(fb->fb_fd);
		return 4;
	}

	return 0;
}

void eosFrameBuffer_Close(eosFrameBuffer* fb)
{
	// Unmap framebuffer memory.
	munmap(fb->buffer, fb->buffer_size);
        
	// Close file descriptor.
	close(fb->fb_fd);
}

void eosFrameBuffer_Draw(eosFrameBuffer* fb)
{
	// Assuming 32 bits color.
	if(fb->fb_var.bits_per_pixel != 32) {
		printf("Error : Can only draw 32 bits framebuffer.\n");
		return;
	}
	
	const int byte_per_pixel = 4; 
	const int xoffset = fb->fb_var.xoffset; 
	const int yoffset = fb->fb_var.yoffset;  
	const int line_byte = fb->fb_fix.line_length;
	unsigned char* buf = (unsigned char*)fb->buffer;

	int y;
	int x;
	int mem_index;
		
	for (y = 0; y < 768; y++) {
        	for (x = 0; x < 1366; x++) {
			mem_index = ((y + yoffset) * line_byte) + ((x + xoffset) * byte_per_pixel);			

                	*(buf + mem_index) = (unsigned char)255;   // Blue.
                	*(buf + mem_index + 1) = (unsigned char)0; // Red.
                	*(buf + mem_index + 2) = (unsigned char)0; // Green.
                	*(buf + mem_index + 3) = (unsigned char)0; // alpha.
        	}
	}
}

void eosFrameBuffer_DrawBackBuffer(eosFrameBuffer* fb, CFrameBuffer* bf)
{
	// Assuming 32 bits color.
	if(fb->fb_var.bits_per_pixel != 32) {
		printf("Error : Can only draw 32 bits framebuffer.\n");
		return;
	}
	
	const int byte_per_pixel = 4; 
	const int xoffset = fb->fb_var.xoffset; 
	const int yoffset = fb->fb_var.yoffset;  
	const int line_byte = fb->fb_fix.line_length;
	unsigned char* buf = (unsigned char*)fb->buffer;

	int y;
	int x;
	int mem_index;
	int bf_index;
	for (y = 0; y < 768; y++) {
        	for (x = 0; x < 1366; x++) {
			mem_index = ((y + yoffset) * line_byte) + ((x + xoffset) * byte_per_pixel);			
			bf_index = y * bf->w + x;
			*((CColor*)(buf + mem_index)) = bf->data[bf_index];			
	
                	//*(buf + mem_index) = (unsigned char)bf->data[];   // Blue.
                	//*(buf + mem_index + 1) = (unsigned char)0; // Red.
                	//*(buf + mem_index + 2) = (unsigned char)0; // Green.
                	//*(buf + mem_index + 3) = (unsigned char)0; // alpha.
        	}
	}
}
int main()
{
	eosFrameBuffer fb;
	if(eosFrameBuffer_Open(&fb)) {
		return 0;
	}

	CFrameBuffer bf = CreateFrameBuffer(1366, 768);
	
	// Draw background rectangle.
	CRect bg_rect = CreateRect(100, 100, 200, 200);
	CColor bg_color = CreateColor(240, 0, 0);
	DrawRectangle(&bf, &bg_rect, &bg_color);

	// Draw rectangle contour.
	//CColor contour_color = CreateColor(0, 0, 0);
	//DrawRectangleContour(&bf, &bg_rect, &contour_color);

	// Draw diagonal line.
	//CPoint p0 = CPoint_Create(0, 499);
	//CPoint p1 = CPoint_Create(499, 0);
	//CColor p_color = CreateColor(255, 255, 0);
	//DrawLine(&bf, &p0, &p1, &p_color);

	// Draw char.
	CFont font = CreateFont("resources/test.myfont");
	//CPoint char_pos = CPoint_Create(300, 100);
	//CColor char_color = CreateColor(255, 255, 255);
	//DrawChar(&bf, &font, 'P', &char_pos, &char_color);

	// Draw text.
	CPoint str_pos = CPoint_Create(100, 200);
	CColor str_color = CreateColor(180, 180, 180);
	DrawString(&bf, &font, "Monster Truck", &str_pos, &str_color);

	// Draw text.
	CPoint str2_pos = CPoint_Create(100, 220);
	CColor str2_color = CreateColor(10, 180, 180);
	DrawString(&bf, &font, "Monster Truck", &str2_pos, &str2_color);

	// Get text width.
	//CString mst_str = CString_Create("Monster Truck");
	//printf("Monster Truck width : %d\n", GetStringXSize(&font, &mst_str));

	// Draw image resize.
	CImage image = CreateImageFromBitmap("resources/umbrella2.bmp");
	CPoint img_pos = CPoint_Create(51, 51);
	CSize img_size = CreateSize(99, 99);
	DrawImageResize(&bf, &image, &img_pos, &img_size);	

	eosFrameBuffer_Draw(&fb);
	eosFrameBuffer_DrawBackBuffer(&fb, &bf);
	eosFrameBuffer_Close(&fb);

	//printf("FIX:\n");
	//printf("FB len         : %d\n", m_FixInfo.smem_len);
	//printf("FB type        : %d\n", m_FixInfo.type);
	//printf("FB type aux    : %d\n", m_FixInfo.type_aux);
	//printf("FB visual      : %d\n", m_FixInfo.visual);
	//printf("FB xpanstep    : %d\n", m_FixInfo.xpanstep);
	//printf("FB ypanstep    : %d\n", m_FixInfo.ypanstep);
	//printf("FB ywrapstep   : %d\n", m_FixInfo.ywrapstep);
	//printf("FB line_length : %d\n", m_FixInfo.line_length);

	//printf("VAR:\n");
	//printf("Res            : %d %d\n", m_VarInfo.xres, m_VarInfo.yres);
	//printf("Res virtual    : %d %d\n", m_VarInfo.xres_virtual, m_VarInfo.yres_virtual);
	//printf("Offset         : %d %d\n", m_VarInfo.xoffset, m_VarInfo.yoffset);
	//printf("bits_per_pixel : %d\n", m_VarInfo.bits_per_pixel);
	//printf("grayscale      : %d\n", m_VarInfo.grayscale);
	return 0;
}
