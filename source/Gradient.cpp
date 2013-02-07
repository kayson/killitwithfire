#include "Gradient.h"

Gradient::Gradient(){}

Vector3 Gradient::getGradient(const Grid &g, const int i, const int j, const int k, Discretization &d)
{
	return Vector3(d.calcDx(g,i,j,k), d.calcDy(g,i,j,k), d.calcDz(g,i,j,k));
}