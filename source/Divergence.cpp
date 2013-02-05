#include "Divergence.h"

Divergence::Divergence(){}

double Divergence::getDivergence(Grid<double> &g, int i, int j, int k, CentralDiff &c)
{
	// G�r central differentiation h�r genom c.calcDx(g, i, j, k) osv, t.ex:
	return c.calcDx(g,i,j,k) + c.calcDy(g,i,j,k) + c.calcDz(g,i,j,k);
	//return 0;
}

double Divergence::getDivergence(Grid<double> &g, int i, int j, int k, UpwindDiff &u)
{
	// Anropa upwind differentiation h�r genom u.calcDx(g, i, j, k) osv, t.ex:
	return u.calcDx(g,i,j,k) + u.calcDy(g,i,j,k) + u.calcDz(g,i,j,k);
	//return 0;
}