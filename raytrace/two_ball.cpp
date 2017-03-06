#include <math.h>
#include "vector.h"
#include "vector.cpp"
#include "ray.h"
#include "ray.cpp"
#include "sphere.h"
#include "sphere.cpp"
#include "board.h"
#include "board.cpp"

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
	
	//set background
	vector vtxsmall;
	vtxsmall.x = -1000;
	vtxsmall.y = -800;
	vtxsmall.z = 1800;
	
	vector vtxbig;
	vtxbig.x = 1500;
	vtxbig.y = 800;
	vtxbig.z = 1800;
	
	board checkboard;
	checkboard.vtxs = vtxsmall;
	checkboard.vtxb = vtxbig;
	
	
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
			
			//third test, a board with a phong ball;
			
			double distance_board;
			double distance_ball;
			
			distance_board = checkboard.raylength(camry);
			distance_ball = ball.raylength(camry);
			
			if (distance_ball < 9998 or distance_board < 9998) {
				
				//shot at the ball
				if (distance_ball < distance_board) {
					img[(iy*800 + ix)*4] = img[(iy*800 + ix)*4] + 5;
					cur_cam_hit = camry.hitpoint(distance_ball);
					bulb_direct = cur_cam_hit.subtract(bulb);
					bulb_direct = bulb_direct.normalize();
					
					bulbray.startpoint = bulb;
					bulbray.direction = bulb_direct;
					
					//no block of bulbray
					if (checkboard.raylength(bulbray)>ball.raylength(bulbray)) {
						double tempa = ball.raylength(bulbray);
						vector pspoint = bulbray.hitpoint(tempa);
						
						if (pspoint.z - 0.1 < cur_cam_hit.z < pspoint.z + 0.1) {
							double phong;
							
							vector nnn = bulbray.direction.add(camry.direction);
							vector lll = cur_cam_hit.subtract(ball.center);
							nnn = nnn.normalize();
							lll = lll.normalize();
							
							phong = nnn.dot(lll);
							phong = phong * phong;
							
							img[(iy*800 + ix)*4 + 0] = img[(iy*800 + ix)*4 + 0] + 250 * phong;
							img[(iy*800 + ix)*4 + 1] = img[(iy*800 + ix)*4 + 1] + 200 * phong;
							img[(iy*800 + ix)*4 + 2] = img[(iy*800 + ix)*4 + 2] + 200 * phong;
						}
					}
					//bulbray is blocked
					//else {}
				}
				else {
					
					//shot at the checkboard
					img[(iy*800 + ix)*4 + 0] = img[(iy*800 + ix)*4 + 0] + 15;
					img[(iy*800 + ix)*4 + 1] = img[(iy*800 + ix)*4 + 1] + 15;
					img[(iy*800 + ix)*4 + 2] = img[(iy*800 + ix)*4 + 2] + 15;
					
					cur_cam_hit = camry.hitpoint(distance_board);
					bulb_direct = cur_cam_hit.subtract(bulb);
					bulb_direct = bulb_direct.normalize();
					
					bulbray.startpoint = bulb;
					bulbray.direction = bulb_direct;
					
					//bulbray is not blocked
					if (ball.raylength(bulbray) > checkboard.raylength(bulbray)) {
						double phong;
						
						vector nnn = bulbray.direction.add(camry.direction);
						/*vector lll;
						lll.x = 0;
						lll.y = 0;
						lll.z = 1;
						*/
						nnn = nnn.normalize();
						
						phong = nnn.z;
						phong = phong*phong;
						
						//white
						if (checkboard.color(camry) > 0) {
							img[(iy*800 + ix)*4 + 0] = img[(iy*800 + ix)*4 + 0] + 200 * phong + 40;
							img[(iy*800 + ix)*4 + 1] = img[(iy*800 + ix)*4 + 1] + 200 * phong + 40;
							img[(iy*800 + ix)*4 + 2] = img[(iy*800 + ix)*4 + 2] + 200 * phong + 40;
						}
						//black
						else {
							img[(iy*800 + ix)*4 + 0] = img[(iy*800 + ix)*4 + 0] + 100 * phong;
							img[(iy*800 + ix)*4 + 1] = img[(iy*800 + ix)*4 + 1] + 100 * phong;
							img[(iy*800 + ix)*4 + 2] = img[(iy*800 + ix)*4 + 2] + 100 * phong;
						}
						
					}
					//block bulbray
					else {
						if (checkboard.color(camry) > 0) {
							img[(iy*800 + ix)*4 + 0] = img[(iy*800 + ix)*4 + 0] + 40;
							img[(iy*800 + ix)*4 + 1] = img[(iy*800 + ix)*4 + 1] + 40;
							img[(iy*800 + ix)*4 + 2] = img[(iy*800 + ix)*4 + 2] + 40;
						}
					}
				}
			}
			
			/*
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
			}*/
			
			
			
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