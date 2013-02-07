#ifndef GRADIENT_H
#define GRADIENT_H
#include "Grid.h"
#include "Discretization.h"

#include "Vector3.h"

class Gradient
{
public:
	Gradient();
	static Vector3 getGradient(const Grid &g, const int i, const int j, const int k, Discretization &d);
};
#endif