#include "sphere.h"

using namespace std;

double sphere::raylength(ray laz) {
	vector cs = laz.startpoint.subtract(center);
	double delta = (laz.direction.dot(laz.startpoint) - laz.direction.dot(center))*(laz.direction.dot(laz.startpoint) - laz.direction.dot(center)) - cs.dot(cs) + radius*radius;
	
	if (delta > 0) {
		return laz.direction.dot(center) - laz.direction.dot(laz.startpoint) - sqrt(delta);
	}
	
	return 9999;
}