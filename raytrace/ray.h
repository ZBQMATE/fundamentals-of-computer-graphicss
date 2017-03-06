#pragma once

class ray {
	// f(t) = o + td
	public:
		
		vector startpoint;
		vector direction;
		vector hitpoint(double);
};