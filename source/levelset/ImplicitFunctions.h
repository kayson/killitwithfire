#ifndef IMPLICITFUNCTIONS_H
#define IMPLICITFUNCTIONS_H
#include "../fire.h"
#include "../presets/firePresets.h"
#include <cmath>
namespace implicitFunction
{
	double sphere(int i, int j, int k)
	{
		double x = (double) i * (double) FirePresets::dx;
		double y = (double) j * (double) FirePresets::dx;
		double z = (double) k * (double) FirePresets::dx;

		double r = (double) 20.0 * (double) FirePresets::dx;
		double xo =  -(double)FirePresets::GRID_DIM_X * ((double) FirePresets::dx / 8) - 5;
		double yo =  -(double)FirePresets::GRID_DIM_Y * ((double) FirePresets::dx / 2);
		double zo =  -(double)FirePresets::GRID_DIM_Z * ((double) FirePresets::dx / 2);
		x += xo;
		y += yo;
		z += zo;
		return sqrt(x * x + y * y + z * z) - sqrt(r * r);
	}
}

#endif //IMPLICITFUNCTIONS_H