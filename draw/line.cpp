#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <windows.h>
/*
typedef struct {
	BYTE r;
	BYTE g;
	BYTE b;
}pixel;


void pin(pixel pxl) {
	pxl.r = 255;
	pxl.g = 255;
	pxl.b = 255;
}
*/


int drawline(int x0, int y0, int x1, int y1) {
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;
	
	FILE *pic = fopen("./cp.bmp", "rb");
	FILE *picout = fopen("./drawline.bmp", "wb");
	
	fread(&fileheader, sizeof(BITMAPFILEHEADER), 1, pic);
	fread(&infoheader, sizeof(BITMAPINFOHEADER), 1, pic);
	//fread(&plate, sizeof(Palette), 1, pic);
	
	int height = infoheader.biHeight;
	int width = infoheader.biWidth;
	int picsize = height * width;
	
	//pixel img[height][width];
	//fread(img, picsize, sizeof(pixel), pic);
	
	//create a blank data field
	unsigned char img[height*width*4];
	
	for (int i = 0; i < height*width*4; i++) {
				img[i] = 0;
	}
	
	
	
	//draw a line
	int yyy = y0;
	/*
	double x0d = (double)x0;
	double x1d = (double)x1;
	double y0d = (double)y0;
	double y1d = (double)y1;
	
	double d = -(y1d - y0d) * (x0d + 1) + (x1d - x0d) * (y0d + 0.5) + x0d * y1d - x1d * y0d;
	*/
	
	int d = -2*(y1-y0)*(x0+1) + (x1-x0)*(2*y0+1) + 2*x0*y1 - 2*x1*y0;
	
	for (int i = x0; i <= x1; i++) {
		//pin(img[i][yyy]);
		
			img[(i*width+yyy)*4+0] = 250;
			img[(i*width+yyy)*4+1] = 250;
			img[(i*width+yyy)*4+2] = 250;
			//img[(i*width+yyy)*4+3] = 250;
			
		if (d < 0) {
			yyy = yyy + 1;
			d = d + 2*(x1 - x0);
		}
		d = d - 2*(y1 - y0);
	}
	infoheader.biBitCount = 32;
	fwrite(&fileheader, sizeof(BITMAPFILEHEADER), 1, picout);
	fwrite(&infoheader, sizeof(BITMAPINFOHEADER), 1, picout);
	fwrite(img, picsize, 4, picout);
	//fwrite(img, picsize, sizeof(pixel), picout);
	fclose(pic);
	fclose(picout);
	
	return 0;
}

int main() {
	drawline(3, 1, 39, 29);
}