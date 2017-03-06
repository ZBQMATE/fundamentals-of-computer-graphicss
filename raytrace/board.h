#pragma once
// board is orthogno with z axis

class board {
	
	public:
		vector vtxs;//smaller indeces
		vector vtxb;
		double num;//check numbers per line
		
		double raylength(ray);
		double color(ray);
};