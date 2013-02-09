#ifndef IMPLICITFUNCTIONS_H
#define IMPLICITFUNCTIONS_H
#include "datatype.h"
namespace implicitFunction
{
	data sphere(int x, int y, int z)
	{
		data r = (data) 3;
		data xo = (data) -GRID_DIM_X / 2;
		data yo = (data) -GRID_DIM_Y / 2;
		data zo = (data) -GRID_DIM_Z / 2;
		x += xo;
		y += yo;
		z += zo;
		return x * x + y * y + z * z - r * r;
	}
}

#endif //IMPLICITFUNCTIONS_H