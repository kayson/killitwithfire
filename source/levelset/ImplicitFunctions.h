#ifndef IMPLICITFUNCTIONS_H
#define IMPLICITFUNCTIONS_H
#include "../fire.h"
#include "../presets/firePresets.h"
namespace implicitFunction
{
	double sphere(int i, int j, int k)
	{
		double x = (double) i / (double) FirePresets::dx;
		double y = (double) j / (double) FirePresets::dx;
		double z = (double) k / (double) FirePresets::dx;

		double r = (double) 100;
		double xo =  -(double)FirePresets::GRID_DIM_X;
		double yo =  -(double)FirePresets::GRID_DIM_Y;
		double zo =  -(double)FirePresets::GRID_DIM_Z;
		x += xo;
		y += yo;
		z += zo;
		return x * x + y * y + z * z - r * r;
	}
}

#endif //IMPLICITFUNCTIONS_H