#include "ray.h"

using namespace std;

vector ray::hitpoint(double t) {
	vector rst;
	rst = startpoint.add(direction.multicons(t));
	return rst;
}