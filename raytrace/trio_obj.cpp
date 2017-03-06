#include <math.h>
#include "vector.h"
#include "vector.cpp"
#include "ray.h"
#include "ray.cpp"
#include "sphere.h"
#include "sphere.cpp"
#include "board.h"
#include "board.cpp"
#include "crystal.h"
#include "crystal.cpp"

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
	ballcenter.x = 300;
	ballcenter.y = 50;
	ballcenter.z = 1300;
	
	double ballradius = 200;
	
	sphere ball;
	ball.center = ballcenter;
	ball.radius = ballradius;
	
	//set object crystal
	
	vector crystalcenter;
	crystalcenter.x = 420;
	crystalcenter.y = -230;
	crystalcenter.z = 1000;
	
	
	double crystalradius = 200;
	
	double crystalrefraction = 1.25;
	
	crystal glass_ball;
	glass_ball.center = crystalcenter;
	glass_ball.radius = crystalradius;
	glass_ball.refraction_rate = crystalrefraction;
	
	double sample_value = 0;
	ray ppp;

	
	for (int iy = 0; iy < 600; iy++) {
		for (int ix = 0; ix < 800; ix++) {
			
			//set current pixel the camera is scaning
			curpix.x = (double)ix - 400;
			curpix.y = (double)iy - 300;
			
			//set current camera ray direction
			camera_direct = curpix.normalize();
			
			camry.startpoint = camera;
			camry.direction = camera_direct;
			
			
			//forth test, crystal ball
			
			double distance_board;
			double distance_ball;
			double distance_glass_ball;
			
			distance_board = checkboard.raylength(camry);
			distance_ball = ball.raylength(camry);
			distance_glass_ball = glass_ball.raylength(camry);
			
			if (distance_ball < 9998 or distance_board < 9998 or distance_glass_ball < 9998) {
				
				//shot at the glass ball
				//if (true) {
				if (distance_glass_ball < distance_ball & distance_glass_ball < distance_board) {
					//img[(iy*800 + ix)*4] = img[(iy*800 + ix)*4] + 5;
					
					ray out_glass = glass_ball.outshot(camry);
					
					/*
					if (camry.direction.y - 0.0001<out_glass.direction.y & out_glass.direction.y< camry.direction.y+0.0001) {
						return 0;
					}
					*/
					double in_ball_length = glass_ball.inside_trace_length(camry);
					//double sample_value = 0;
					
					double ball_out_glass = ball.raylength(out_glass);
					double board_out_glass = checkboard.raylength(out_glass);
					
					//if (true) {
					if (ball_out_glass < 9998 or board_out_glass < 9998) {
						
						//refract to ball
						if (ball_out_glass < board_out_glass) {
							vector refract_hit = out_glass.hitpoint(ball_out_glass);
							bulb_direct = refract_hit.subtract(bulb);
							bulb_direct = bulb_direct.normalize();
							
							bulbray.startpoint = bulb;
							bulbray.direction = bulb_direct;
							
							//no block
							if (ball.raylength(bulbray) < checkboard.raylength(bulbray) & ball.raylength(bulbray) < glass_ball.raylength(bulbray)) {
								double tempa = ball.raylength(bulbray);
								vector pspoint = bulbray.hitpoint(tempa);
								
								if (pspoint.z - 0.1 < refract_hit.z < pspoint.z + 0.1) {
									double phong;
									
									vector nnn = bulbray.direction.add(out_glass.direction);
									vector lll = refract_hit.subtract(ball.center);
									nnn = nnn.normalize();
									lll = lll.normalize();
									
									phong = nnn.dot(lll);
									phong = phong * phong;
									
									//sample_value = 0;
									sample_value = 250 * phong + 1/(in_ball_length + 0.1);
								}
								else {
									sample_value = 10 + 1/(in_ball_length + 0.1);
								}
							}
							//block
							else {
								sample_value = 10 + 1/(in_ball_length + 0.1);
							}
						}
						//refract to board
						else {
							vector refract_hit = out_glass.hitpoint(board_out_glass);
							bulb_direct = refract_hit.subtract(bulb);
							bulb_direct = bulb_direct.normalize();
							
							bulbray.startpoint = bulb;
							bulbray.direction = bulb_direct;
							
							//board is not blocked
							if (checkboard.raylength(bulbray) < ball.raylength(bulbray) & checkboard.raylength(bulbray) < glass_ball.raylength(bulbray)) {
								double phong;
						
								vector nnn = bulbray.direction.add(out_glass.direction);
								nnn = nnn.normalize();
								
								phong = nnn.z;
								phong = phong*phong;
								//damm!
								//ray ppp;
								vector sp;
								sp.x = 0;
								sp.y = 0;
								sp.z = 0;
								ppp.startpoint = sp;
								ppp.direction = refract_hit.normalize();
								
								//white
								if (checkboard.color(ppp) > 0) {
									//sample_value = 0;
									sample_value = 200 * phong + 40 + 1/(in_ball_length + 0.1);
								}
								//black
								else {
									sample_value = 100 * phong + 1/(in_ball_length + 0.1);
								}
								
							}
							//blocked
							else {
								//white
								if (checkboard.color(ppp) > 0) {
									sample_value = 40 + 1/(in_ball_length + 0.1);
								}
								//black
								else {
									sample_value = 10 + 1/(in_ball_length + 0.1);
								}
							}
						}
					}
					//refract to none
					else {
						sample_value = 0;
					}
				}
				
				//shot at the ball
				if (distance_ball < distance_board & distance_ball < distance_glass_ball) {
					//img[(iy*800 + ix)*4] = img[(iy*800 + ix)*4] + 5;
					
					cur_cam_hit = camry.hitpoint(distance_ball);
					bulb_direct = cur_cam_hit.subtract(bulb);
					bulb_direct = bulb_direct.normalize();
					
					bulbray.startpoint = bulb;
					bulbray.direction = bulb_direct;
					
					//no block of bulbray
					if (checkboard.raylength(bulbray) > ball.raylength(bulbray) & glass_ball.raylength(bulbray) > ball.raylength(bulbray)) {
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
							
							sample_value = 250 * phong;
						}
						else {
							sample_value = 10;
						}
					}
					//bulbray is blocked
					else {
						sample_value = 10;
					}
				}
				
				//shot at the board
				if (distance_ball > distance_board & distance_board < distance_glass_ball) {
					
					cur_cam_hit = camry.hitpoint(distance_board);
					bulb_direct = cur_cam_hit.subtract(bulb);
					bulb_direct = bulb_direct.normalize();
					
					bulbray.startpoint = bulb;
					bulbray.direction = bulb_direct;
					
					//bulbray is not blocked
					if (ball.raylength(bulbray) > checkboard.raylength(bulbray) & glass_ball.raylength(bulbray) > checkboard.raylength(bulbray)) {
						double phong;
						
						vector nnn = bulbray.direction.add(camry.direction);
						
						nnn = nnn.normalize();
						
						phong = nnn.z;
						phong = phong*phong;
						
						//white
						if (checkboard.color(camry) > 0) {
							sample_value = 200 * phong + 55;
						}
						//black
						else {
							sample_value = 100 * phong + 15;
						}
						
					}
					//block bulbray
					else {
						if (checkboard.color(camry) > 0) {
							sample_value = 40;
						}
						else {
							sample_value = 15;
						}
						//sample_value = sample_value + 15;
					}
					
				}
				
				img[(iy*800 + ix)*4 + 0] = img[(iy*800 + ix)*4 + 0] + sample_value;
				img[(iy*800 + ix)*4 + 1] = img[(iy*800 + ix)*4 + 1] + sample_value;
				img[(iy*800 + ix)*4 + 2] = img[(iy*800 + ix)*4 + 2] + sample_value * 0.8;
			}
			//$$$$$$$$$
			//third test, a board with a phong ball;
			/*
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
			*/
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