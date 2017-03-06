#pragma once


class vector {
	
	public:
		double x;
		double y;
		double z;
		
		//vector(int, int, int); constructor
		vector copy();
		double length();
		vector normalize();
		vector add(vector);
		vector subtract(vector);
		vector multicons(double);
		double dot(vector);
		vector cross(vector);
};