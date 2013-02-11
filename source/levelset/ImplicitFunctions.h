#ifndef IMPLICITFUNCTIONS_H
#define IMPLICITFUNCTIONS_H
#include "datatype.h"
#include "../fire.h"
namespace implicitFunction
{
	data sphere(int x, int y, int z)
	{
		data r = (data) 10;
		data xo = (data) -FirePresets::GRID_DIM_X / 2.0;
		data yo = (data) -FirePresets::GRID_DIM_Y / 2.0;
		data zo = (data) -FirePresets::GRID_DIM_Z / 2.0;
		x += xo;
		y += yo;
		z += zo;
		return x * x + y * y + z * z - r * r;
	}
}

#endif //IMPLICITFUNCTIONS_H