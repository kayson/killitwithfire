#include <iostream>
#include "Grid.h"
#include "Discretization.h"
#include "CentralDiff.h"
#include "UpwindDiff.h"

class Divergence
{
public:
	Divergence();

	double getDivergence(Grid &g, int i, int j, int k, Discretization &c);

};