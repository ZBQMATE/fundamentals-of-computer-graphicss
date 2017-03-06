#include "vector.h"

using namespace std;

vector vector::copy() {
	vector rt;
	rt.x = x;
	rt.y = y;
	rt.z = z;
	return rt;
}

double vector::length() {
	return sqrt(x*x + y*y +z*z);
}

vector vector::normalize() {
	vector rt;
	rt.x = x/sqrt(x*x + y*y +z*z);
	rt.y = y/sqrt(x*x + y*y +z*z);
	rt.z = z/sqrt(x*x + y*y +z*z);
	return rt;
}

vector vector::add(vector ant) {
	vector rt;
	rt.x = x+ant.x;
	rt.y = y+ant.y;
	rt.z = z+ant.z;
	return rt;
}

vector vector::subtract(vector ant) {
	vector rt;
	rt.x = x-ant.x;
	rt.y = y-ant.y;
	rt.z = z-ant.z;
	return rt;
}

vector vector::multicons(double plr) {
	vector rt;
	rt.x = x*plr;
	rt.y = y*plr;
	rt.z = z*plr;
	return rt;
}

double vector::dot(vector dtr) {
	return x*dtr.x + y*dtr.y + z*dtr.z;
}

vector vector::cross(vector ant) {
	vector rt;
	rt.x = y*ant.z - z*ant.y;
	rt.y = z*ant.x - x*ant.z;
	rt.z = x*ant.y - y*ant.x;
	return rt;
}