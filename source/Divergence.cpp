#include "Divergence.h"

Divergence::Divergence(){}

double Divergence::getDivergence(Grid<double> &g, int i, int j, int k, Discretization &d)
{
	return d.calcDx(g,i,j,k) + d.calcDy(g,i,j,k) + d.calcDz(g,i,j,k);
}