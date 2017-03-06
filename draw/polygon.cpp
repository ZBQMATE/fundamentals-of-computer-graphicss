#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include <vector>
#include <windows.h>
using namespace std;

class edge {
	public:
	double aax;
	double bbx;
	double aay;
	double bby;
};

int drawpolygon(int xa, int ya, int xb, int yb, int xc, int yc, int xd, int yd) {
	
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;
	
	FILE *pic = fopen("./cp.bmp", "rb");
	FILE *picout = fopen("./drawpolygon.bmp", "wb");
	
	fread(&fileheader, sizeof(BITMAPFILEHEADER), 1, pic);
	fread(&infoheader, sizeof(BITMAPINFOHEADER), 1, pic);
	
	int height = infoheader.biHeight;
	int width = infoheader.biWidth;
	int picsize = height * width;
	
	unsigned char img[height*width*4];
	
	for (int i = 0; i < height*width*4; i++) {
		img[i] = 120;
	}
	
	
	
	//draw
	
	double dxa = (double)xa;
	double dya = (double)ya;
	double dxb = (double)xb;
	double dyb = (double)yb;
	double dxc = (double)xc;
	double dyc = (double)yc;
	double dxd = (double)xd;
	double dyd = (double)yd;
	
	edge ab;
	ab.aax = dxa;
	ab.bbx = dxb;
	ab.aay = dya;
	ab.bby = dyb;
	
	edge ac;
	ac.aax = dxa;
	ac.bbx = dxc;
	ac.aay = dya;
	ac.bby = dyc;
	
	edge bd;
	bd.aax = dxb;
	bd.bbx = dxd;
	bd.aay = dyb;
	bd.bby = dyd;
	
	edge cd;
	cd.aax = dxc;
	cd.bbx = dxd;
	cd.aay = dyc;
	cd.bby = dyd;
	
	edge nu;
	nu.aax = 0;
	nu.bbx = 0;
	nu.aay = 0;
	nu.bby = 0;
	
	edge all[4] = {ab, ac, bd, cd};
	edge temp[10] = {nu, nu, nu, nu, nu, nu, nu, nu, nu, nu};
	//vector<edge*> active;
	
	double lowy = 0;
	double highy = 0;
	int crossdots[10];
	for (int pp = 0; pp < 10; pp++) {
		crossdots[pp] = 0;
	}
	double x0 = 0;
	double y0 = 0;
	double x1 = 0;
	double y1 = 0;
	double di = 0;
	double dw = 0;
	int zf = 0;
	edge ccc;
	int act = 0;
	
	for (int i = 0; i < height; i++) {
		
		for (int jj = 0; jj < 4; jj++) {
			
			ccc = all[jj];
			lowy = ccc.aay;
			highy = ccc.bby;
			
			if (lowy > highy) {
				lowy = highy;
				highy = ccc.aay;
			}
			
			if (i == (int)lowy) {
				temp[act] = ccc;
				act = act + 1;
			}
			
			if (i == (int)highy) {
				
				for (int j = 0; j < 10; j++) {
					
					if (temp[j].aax == ccc.aax & temp[j].bbx == ccc.bbx & temp[j].aay == ccc.aay & temp[j].bby == ccc.bby) {
						temp[j] = nu;
						//active.erase(active.begin()+j);
					}
					
				}
				
			}
			/**/
		}
		
		
		
		for (int h = 0; h < 10; h++) {
			x0 = temp[h].aax;
			x1 = temp[h].bbx;
			y0 = temp[h].aay;
			y1 = temp[h].bby;
			di = (double)i;
			if (y1-y0 != 0) {
				crossdots[h] = (int)x1 - (y1-di)*(x1-x0)/(y1-y0);
			}
		}
		
		for (int w = 0; w < 10; w=w+1) {
			dw = (double)w;
			zf = (int)pow(-1,dw);
			for (int sss = 0; sss < crossdots[w]; sss++) {
				img[(i*width+sss)*4+0] = img[(i*width+sss)*4+0] - 100*zf;
				img[(i*width+sss)*4+1] = img[(i*width+sss)*4+1] - 100*zf;
				img[(i*width+sss)*4+2] = img[(i*width+sss)*4+2] - 100*zf;
			}
		}
		
		for (int pp = 0; pp < 10; pp++) {
			crossdots[pp] = 0;
		}
		/**/
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
	drawpolygon(3, 4, 33, 7, 11, 39, 41, 42);
}