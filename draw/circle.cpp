#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <windows.h>

int drawcircle(int x, int y, int r) {
	
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;
	
	FILE *pic = fopen("./cp.bmp", "rb");
	FILE *picout = fopen("./drawcircle.bmp", "wb");
	
	fread(&fileheader, sizeof(BITMAPFILEHEADER), 1, pic);
	fread(&infoheader, sizeof(BITMAPINFOHEADER), 1, pic);
	
	int height = infoheader.biHeight;
	int width = infoheader.biWidth;
	int picsize = height * width;
	
	unsigned char img[height*width*4];
	
	for (int i = 0; i < height*width*4; i++) {
		img[i] = 0;
	}
	
	//draw circle
	
	double xr = (double)x + r;
	double xd = (double)x;
	double yd = (double)y;
	double rd = (double)r;
	double dis = (xr-xd)*(xr-xd) - rd*rd;
	
	for (double yyy = yd; yyy < yd + 0.707*rd;) {
		
		int intx = (int)xr;
		int inty = (int)yyy;
		int xc = x;
		int yc = y;
		
		img[(intx*width+inty)*4+0] = 240;
		img[(intx*width+inty)*4+1] = 240;
		img[(intx*width+inty)*4+2] = 240;
		
		img[((xc+inty-yc)*width+yc+intx-xc)*4+0] = 240;
		img[((xc+inty-yc)*width+yc+intx-xc)*4+1] = 240;
		img[((xc+inty-yc)*width+yc+intx-xc)*4+2] = 240;
		
		img[((xc-inty+yc)*width+yc+intx-xc)*4+0] = 240;
		img[((xc-inty+yc)*width+yc+intx-xc)*4+1] = 240;
		img[((xc-inty+yc)*width+yc+intx-xc)*4+2] = 240;
		
		img[((xc-intx+xc)*width+inty)*4+0] = 240;
		img[((xc-intx+xc)*width+inty)*4+1] = 240;
		img[((xc-intx+xc)*width+inty)*4+2] = 240;
		
		img[((xc-intx+xc)*width+yc-inty+yc)*4+0] = 240;
		img[((xc-intx+xc)*width+yc-inty+yc)*4+1] = 240;
		img[((xc-intx+xc)*width+yc-inty+yc)*4+2] = 240;
		
		img[((xc-inty+yc)*width+yc-intx+xc)*4+0] = 240;
		img[((xc-inty+yc)*width+yc-intx+xc)*4+1] = 240;
		img[((xc-inty+yc)*width+yc-intx+xc)*4+2] = 240;
		
		img[((xc+inty-yc)*width+yc-intx+xc)*4+0] = 240;
		img[((xc+inty-yc)*width+yc-intx+xc)*4+1] = 240;
		img[((xc+inty-yc)*width+yc-intx+xc)*4+2] = 240;
		
		img[(intx*width+yc-inty+yc)*4+0] = 240;
		img[(intx*width+yc-inty+yc)*4+1] = 240;
		img[(intx*width+yc-inty+yc)*4+2] = 240;
		
		
		if (dis > 0) {
			xr = xr - 1;
			yyy = yyy + 1;
			dis = (xr-xd)*(xr-xd) + (yyy-yd)*(yyy-yd) -rd*rd;
		}
		else {
			yyy = yyy + 1;
			dis = (xr-xd)*(xr-xd) + (yyy-yd)*(yyy-yd) -rd*rd;
		}
	}
	
	//save
	infoheader.biBitCount = 32;
	
	fwrite(&fileheader, sizeof(BITMAPFILEHEADER), 1, picout);
	fwrite(&infoheader, sizeof(BITMAPINFOHEADER), 1, picout);
	fwrite(img, picsize, 4, picout);
	
	fclose(pic);
	fclose(picout);
	
	return 0;
}

int main() {
	drawcircle(20, 20, 12);
}