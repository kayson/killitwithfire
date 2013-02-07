#ifndef UPWINDDIFF_H
#define UPWINDDIFF_H
#include "Discretization.h"

class UpwindDiff : public Discretization{
public:
	UpwindDiff();
	data calcDx(Grid &g, int i, int j, int k);
	data calcDy(Grid &g, int i, int j, int k);
	data calcDz(Grid &g, int i, int j, int k);

	data calcD2x(Grid &g, int i, int j, int k);
	data calcD2y(Grid &g, int i, int j, int k);
	data calcD2z(Grid &g, int i, int j, int k);

	data calcDyz(Grid &g, int i, int j, int k);
	data calcDxz(Grid &g, int i, int j, int k);
	data calcDxy(Grid &g, int i, int j, int k);
};
#endif