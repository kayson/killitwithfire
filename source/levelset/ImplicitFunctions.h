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

		double dx = 0.05;//TODO så man skickar in dx istället!

		double r = (double) 0.75; //Världskoordinater
		double xo = FirePresets::GRID_DIM_X / 2;//Gridkoordinater
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