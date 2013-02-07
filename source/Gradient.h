#ifndef GRADIENT_H
#define GRADIENT_H
#include "Grid.h"
#include "Discretization.h"
#include "armadillo"

class Gradient
{
public:
	Gradient();
	arma::vec getGradient(Grid &g, int i, int j, int k, Discretization &d);
};
#endif