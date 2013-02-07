#include <iostream>
#include "Grid.h"
#include "Discretization.h"
#include "CentralDiff.h"
#include "UpwindDiff.h"
#include "presets\firePresets.h"

class Divergence
{
public:
	Divergence();
	double getDivergence(Grid<double> &g, int i, int j, int k, Discretization &c);
};