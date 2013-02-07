#ifndef GRADIENT_H
#define GRADIENT_H
#include "Grid.h"
#include "Discretization.h"
#include "armadillo"

class Gradient
{
	Gradient();
	arma::vec getGradient(Grid<double> &g, int i, int j, int k, Discretization &d);
};
#endif