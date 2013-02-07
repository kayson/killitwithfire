#ifndef GRADIENT_H
#define GRADIENT_H
#include "Grid.h"
#include "Discretization.h"
#ifdef __APPLE__
#include "armadillo.h"
#elif defined _WIN32 || defined _WIN64
#include "armadillo"
#endif

class Gradient
{
public:
	Gradient();
	static arma::vec getGradient(Grid &g, int i, int j, int k, Discretization &d);
};
#endif