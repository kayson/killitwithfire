#include "Divergence.h"

Divergence::Divergence(){}

double getDivergence(Grid &g, int i, int j, int k, Discretization &c)
{
	return c.calcDx(g,i,j,k) + c.calcDy(g,i,j,k) + c.calcDz(g,i,j,k);
}