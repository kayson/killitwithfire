#ifndef IMPLICITFUNCTIONS_H
#define IMPLICITFUNCTIONS_H

#include "firePresets.h"

#include <cmath>

namespace implicitFunction
{
	double sphere(int i, int j, int k)
	{
        
		double x = i;
		double y = j;
		double z = k;

		// todo send correct dx dy and dz
		double dx = double(FirePresets::GRID_SIZE)/double(FirePresets::GRID_DIM_X); 

		double r = (double) 0.5; // def in world coord
		double xo = FirePresets::GRID_DIM_X / 2;// def in grid coord
		double yo = double(FirePresets::GRID_DIM_Y)*0.2;
		double zo = FirePresets::GRID_DIM_Z / 2;
		x -= xo;
		y -= yo;
		z -= zo;
		return -sqrt(x * x + y * y + z * z)*dx + sqrt(r * r);
	}
	double empty(int i, int j, int k)
	{
		return 0.;
	}
}

#endif //IMPLICITFUNCTIONS_H