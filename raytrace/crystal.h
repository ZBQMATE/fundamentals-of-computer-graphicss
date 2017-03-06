#pragma once

class crystal {
	
	public:
		
		vector center;
		double radius;
		double refraction_rate;
		
		double raylength(ray);
		ray outshot(ray);
		double inside_trace_length(ray);
};