#include "Divergence.h"

Divergence::Divergence(){}


double Divergence::getDivergence(GridField<double> &g, const int i, const int j, const int k, Discretization &d)
{
	return d.calcDx(g,i,j,k) + d.calcDy(g,i,j,k) + d.calcDz(g,i,j,k);
}