#ifndef GRADIENT_H
#define GRADIENT_H
#include "Grid.h"
#include "Discretization.h"

#include "Vector3.h"

class Gradient
{
public:
	Gradient();
	static Vector3 getGradient(Grid &g, int i, int j, int k, Discretization &d);
};
#endif