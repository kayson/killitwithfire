#include "Gradient.h"

Gradient::Gradient(){}

arma::vec Gradient::getGradient(Grid<double> &g, int i, int j, int k, Discretization &d)
{
	arma::vec v(3);
	v(0) = d.calcDx(g,i,j,k);
	v(1) = d.calcDy(g,i,j,k);
	v(2) = d.calcDz(g,i,j,k);

	return v;
}