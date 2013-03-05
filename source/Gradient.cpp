#include "Gradient.h"
#include "Vector3.h"
#include "GridField.hpp"
#include "Discretization.h"

Gradient::Gradient(){}

Vector3 Gradient::getGradient(GridField<double> &g, const int i, const int j, const int k, Discretization &d)
{
	if(g.zdim() != 1)
		return Vector3(d.calcDx(g,i,j,k), d.calcDy(g,i,j,k), d.calcDz(g,i,j,k));
	return Vector3(d.calcDx(g,i,j,k), d.calcDy(g,i,j,k), 0.0);
}