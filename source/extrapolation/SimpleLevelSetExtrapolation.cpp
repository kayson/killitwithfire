/*#include "SimpleLevelSetExtrapolation.h"

#include "GridField.h"
#include "Vector3.h"
#include "firePresetsTwoDimension.h"

double SimpleLevelSetExtrapolation::extrapolate(GridField<double>& g, const int i, const int j, const int k) 
{
	double x1 = double(i);
	double y1 = double(j);
	double z1 = double(k);

	int a = i;
	int b = j;
	int c = k;

	//Hitta n�rmaste punkt via cityblock
	if(a < 0)
		a = 0;
	else if(a >= g.xdim())
		a = g.xdim()-1;

	if(b < 0)
		b = 0;
	else if(b >= g.ydim())
		b = g.ydim()-1;

	if(c < 0)
		c = 0;
	else if(c >= g.zdim())
		c = g.zdim()-1;

	double value = g.valueAtIndex(a, b, c);

	double x0 = double(a);
	double y0 = double(b);
	double z0 = double(c);

	return value - FirePresets::dx*std::sqrt(std::pow(x1-x0, 2.0) + std::pow(y1-y0, 2.0) + std::pow(y1-y0, 2.0));
}

Vector3 SimpleLevelSetExtrapolation::extrapolate(GridField<Vector3>& g, const int i, const int j, const int k)
{
	// EJ DEF IGENTLIGEN, denna hittar n�rmaste cell och s�tter v�rdet till denna.
	int a = i;
	int b = j;
	int c = k;

	//Hitta n�rmaste punkt via cityblock
	if(a < 0)
		a = 0;
	else if(a >= g.xdim())
		a = g.xdim()-1;

	if(b < 0)
		b = 0;
	else if(b >= g.ydim())
		b = g.ydim()-1;

	if(c < 0)
		c = 0;
	else if(c >= g.zdim())
		c = g.zdim()-1;

	return g.valueAtIndex(a, b, c);
}
*/