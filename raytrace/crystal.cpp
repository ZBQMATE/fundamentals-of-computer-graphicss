#include "crystal.h"

using namespace std;

//refraction rate = sinalpha/sinbeta

double crystal::raylength(ray laz) {
	vector cs = laz.startpoint.subtract(center);
	double delta = (laz.direction.dot(laz.startpoint) - laz.direction.dot(center))*(laz.direction.dot(laz.startpoint) - laz.direction.dot(center)) - cs.dot(cs) + radius*radius;
	
	if (delta > 0) {
		return laz.direction.dot(center) - laz.direction.dot(laz.startpoint) - sqrt(delta);
	}
	
	return 9999;
}

ray crystal::outshot(ray laz) {
	//ot shots from the crystal
	//ray ot;
	
	vector in_hit_point;
	double in_cosalpha;
	double in_sinalpha;
	double in_sinbeta;
	
	vector out_hit_point;
	//double out_sinalpha;
	//double out_sinbeta;
	
	double in_t = raylength(laz);
	in_hit_point = laz.hitpoint(in_t);
	vector temp = center.subtract(in_hit_point);
	in_cosalpha = laz.direction.dot(temp.normalize());
	in_sinalpha = sqrt(1 - in_cosalpha * in_cosalpha);
	in_sinbeta = in_sinalpha / refraction_rate;
	
	vector inside_ball;
	
	vector tempb = temp.normalize();
	vector tempc = laz.direction.subtract(tempb.multicons(in_cosalpha));
	vector tempd = tempc.multicons(1/refraction_rate);
	inside_ball = tempd.add(tempb.multicons(in_cosalpha));
	inside_ball = inside_ball.normalize();
	
	double inside_length;
	inside_length = temp.length() * sqrt(1 - in_sinbeta * in_sinbeta) * 2;
	
	out_hit_point = in_hit_point.add(inside_ball.multicons(inside_length));
	/*
	double sin_theta = in_sinalpha * sqrt(1 - in_sinbeta * in_sinbeta) - in_cosalpha * in_sinbeta;
	double cos_theta = sqrt(1 - sin_theta * sin_theta);
	
	double xxx = inside_ball.x * cos_theta + inside_ball.y * sin_theta;
	double yyy = - inside_ball.x * sin_theta + inside_ball.y * cos_theta;
	
	vector out_direction;
	out_direction.x = xxx;
	out_direction.y = yyy;
	*/
	
	vector tpa = out_hit_point.subtract(center);
	tpa = tpa.normalize();
	tpa = tpa.multicons(in_cosalpha);
	vector tpb = inside_ball.subtract(tpa);
	tpb = tpb.multicons(refraction_rate);
	
	vector out_direction = tpb.add(tpa);
	out_direction = out_direction.normalize();
	
	ray ot;
	ot.startpoint = out_hit_point;
	ot.direction = out_direction;
	
	
	//ot.direction.y = -laz.direction.y;
	//return laz;
	return ot;
}

double crystal::inside_trace_length(ray laz) {
	//ot shots from the crystal
	//ray ot;
	
	vector in_hit_point;
	double in_cosalpha;
	double in_sinalpha;
	double in_sinbeta;
	
	//vector out_hit_point;
	//double out_sinalpha;
	//double out_sinbeta;
	
	double in_t = raylength(laz);
	in_hit_point = laz.hitpoint(in_t);
	vector temp = center.subtract(in_hit_point);
	in_cosalpha = laz.direction.dot(temp.normalize());
	in_sinalpha = sqrt(1 - in_cosalpha * in_cosalpha);
	in_sinbeta = in_sinalpha / refraction_rate;
	
	vector inside_ball;
	
	vector tempb = temp.normalize();
	vector tempc = laz.direction.subtract(tempb.multicons(in_cosalpha));
	vector tempd = tempc.multicons(1/refraction_rate);
	inside_ball = tempd.add(tempb.multicons(in_cosalpha));
	inside_ball = inside_ball.normalize();
	
	double inside_length;
	inside_length = temp.length() * sqrt(1 - in_sinbeta * in_sinbeta) * 2;
	
	return inside_length;
}