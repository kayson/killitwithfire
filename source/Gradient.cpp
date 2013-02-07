#include "Gradient.h"

Gradient::Gradient(){}

Vector3 Gradient::getGradient(Grid &g, int i, int j, int k, Discretization &d)
{
	return Vector3(d.calcDx(g,i,j,k), d.calcDy(g,i,j,k), d.calcDz(g,i,j,k));
}