#include <math.h>
#include "vector.h"
#include "vector.cpp"
#include "ray.h"
#include "ray.cpp"
#include "sphere.h"
#include "sphere.cpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <windows.h>


using namespace std;

int photo() {
	
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;
	
	FILE *pic = fopen("./scale.bmp", "rb");
	FILE *picout = fopen("./result.bmp", "wb");
	
	fread(&fileheader, sizeof(BITMAPFILEHEADER), 1, pic);
	fread(&infoheader, sizeof(BITMAPINFOHEADER), 1, pic);
	
	int height = infoheader.biHeight;
	int width = infoheader.biWidth;
	int picsize = height * width;
	
	unsigned char img[height*width*4];
	
	for (int i = 0; i < height*width*4; i++) {
		img[i] = 0;
	}
	
	//process
	
	//set camera at (0,0,0)
	vector camera;
	camera.x = 0;
	camera.y = 0;
	camera.z = 0;
	
	//set canvas vertices
	
	vector vertexa;
	vertexa.x = 400;
	vertexa.y = 300;
	vertexa.z = 500;
	
	vector vertexb;
	vertexb.x = -400;
	vertexb.y = 300;
	vertexb.z = 500;
	
	vector vertexc;
	vertexc.x = -400;
	vertexc.y = -300;
	vertexc.z = 500;
	
	vector vertexd;
	vertexd.x = 400;
	vertexd.y = -300;
	vertexd.z = 500;
	
	//current pixel
	vector curpix;
	curpix.x = 0;
	curpix.y = 0;
	curpix.z = 500;
	
	//current camera ray direction
	vector camera_direct;
	
	//current camera ray
	ray camry;
	
	//current camera hit point
	vector cur_cam_hit;
	
	//set light source
	vector bulb;
	bulb.x = -500;
	bulb.y = -900;
	bulb.z = 500;
	
	//current bulb ray
	ray bulbray;
	
	//current bulb ray direction
	vector bulb_direct;
	
	
	//set boject ball
	vector ballcenter;
	ballcenter.x = 0;
	ballcenter.y = 0;
	ballcenter.z = 1000;
	
	double ballradius = 300;
	
	sphere ball;
	ball.center = ballcenter;
	ball.radius = ballradius;
	

	
	for (int iy = 0; iy < 600; iy++) {
		for (int ix = 0; ix < 800; ix++) {
			
			//set current pixel the camera is scaning
			curpix.x = (double)ix - 400;
			curpix.y = (double)iy - 300;
			
			//set current camera ray direction
			camera_direct = curpix.normalize();
			
			camry.startpoint = camera;
			camry.direction = camera_direct;
			
			//first test, see a ball
			/*
			double distance;
			distance = ball.raylength(camry);
			if (distance < 9998) {
				img[(iy*800 + ix)*4 + 0] = 255;
				img[(iy*800 + ix)*4 + 1] = 255;
				img[(iy*800 + ix)*4 + 2] = 255;
			}
			*/
			
			//second test, phong model
			double distance;
			distance = ball.raylength(camry);
			if (distance < 9998) {
				//img[(iy*800 + ix)*4] = img[(iy*800 + ix)*4] + 5;
				cur_cam_hit = camry.hitpoint(distance);
				bulb_direct = cur_cam_hit.subtract(bulb);
				bulb_direct = bulb_direct.normalize();
				
				bulbray.startpoint = bulb;
				bulbray.direction = bulb_direct;
				
				double tempa = ball.raylength(bulbray);
				vector pspoint = bulbray.hitpoint(tempa);
				//if (pspoint.x == cur_cam_hit.x & pspoint.y == cur_cam_hit.y & pspoint.z == cur_cam_hit.z) {
				//if (pspoint.x-0.1 < cur_cam_hit.x < pspoint.x+0.1 & pspoint.y-0.1 < cur_cam_hit.y < pspoint.y+0.1 & pspoint.z-0.1 < cur_cam_hit.z < pspoint.z+0.1) {
				if (pspoint.z-0.1 < cur_cam_hit.z < pspoint.z+0.1) {
					
					double phong;
					
					vector nnn = bulbray.direction.add(camry.direction);
					vector lll = cur_cam_hit.subtract(ball.center);
					lll = lll.normalize();
					nnn = nnn.normalize();
					
					phong = lll.dot(nnn);
					phong = phong*phong;
					
					img[(iy*800 + ix)*4 + 0] = img[(iy*800 + ix)*4 + 0] + 250 * phong;
					img[(iy*800 + ix)*4 + 1] = img[(iy*800 + ix)*4 + 1] + 250 * phong;
					img[(iy*800 + ix)*4 + 2] = img[(iy*800 + ix)*4 + 2] + 250 * phong;
				}
			}
			
			
			
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
	photo();
}