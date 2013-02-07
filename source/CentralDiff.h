#ifndef CENTRALDIFF_H
#define CENTRALDIFF_H
#include "Discretization.h"

class CentralDiff : public Discretization{
public:
	CentralDiff();
	data calcDx(Grid<data> &g, int i, int j, int k);
	data calcDy(Grid<data> &g, int i, int j, int k);
	data calcDz(Grid<data> &g, int i, int j, int k);

	data calcD2x(Grid<data> &g, int i, int j, int k);
	data calcD2y(Grid<data> &g, int i, int j, int k);
	data calcD2z(Grid<data> &g, int i, int j, int k);

	data calcDyz(Grid<data> &g, int i, int j, int k);
	data calcDxz(Grid<data> &g, int i, int j, int k);
	data calcDxy(Grid<data> &g, int i, int j, int k);
};
#endif