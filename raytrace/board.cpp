#include "board.h"

using namespace std;

double board::raylength(ray laz) {
	double t = vtxs.z - laz.startpoint.z;
	t = t / sqrt(laz.direction.z * laz.direction.z);
	//vector td = laz.direction.multicons(t);
	vector reachpoint = laz.hitpoint(t);
	if (vtxs.x < reachpoint.x & reachpoint.x < vtxb.x & vtxs.y < reachpoint.y & reachpoint.y < vtxb.y) {
		return t;
	}
	return 9999;
}

double board::color(ray laz) {
	double ts = vtxs.z - laz.startpoint.z;
	ts = ts / sqrt(laz.direction.z * laz.direction.z);
	vector reachpoint = laz.hitpoint(ts);
	if (vtxs.x < reachpoint.x & reachpoint.x < vtxb.x & vtxs.y < reachpoint.y & reachpoint.y < vtxb.y) {
		double hor = (reachpoint.x - vtxs.x) / (vtxb.x - vtxs.x);
		double ver = (reachpoint.y - vtxs.y) / (vtxb.y - vtxs.y);
		double hhh = 0;
		double vvv = 0;
		
		if ((0<hor & hor<0.2) | (0.4<hor & hor<0.6) | (0.8<hor & hor<1.0)) {
			hhh = 1;
		}
		else {
			hhh = -1;
		}
		
		if ((0<ver & ver<0.2) | (0.4<ver & ver<0.6) | (0.8<ver & ver<1.0)) {
			vvv = 1;
		}
		else {
			vvv = -1;
		}
		
		return vvv * hhh;
	}
}