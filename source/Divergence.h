#include <iostream>
#include "Grid.h"
#include "Discretization.h"
#include "CentralDiff.h"
#include "UpwindDiff.h"

class Divergence
{
public:
	Divergence();

	static double getDivergence(GridField<double> &g, const int i, const int j, const int k, Discretization &c);

};