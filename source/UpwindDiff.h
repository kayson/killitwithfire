#ifndef UPWINDDIFF_H
#define UPWINDDIFF_H
#include "Discretization.h"
#include "VectorGrid.h"

class UpwindDiff : public Discretization{

public:
	UpwindDiff();

	double calcDx(Grid &g, const int i, const int j, const int k);
	double calcDy(Grid &g, const int i, const int j, const int k);
	double calcDz(Grid &g, const int i, const int j, const int k);

	double calcD2x(Grid &g, const int i, const int j, const int k);
	double calcD2y(Grid &g, const int i, const int j, const int k);
	double calcD2z(Grid &g, const int i, const int j, const int k);

	double calcDyz(Grid &g, const int i, const int j, const int k);
	double calcDxz(Grid &g, const int i, const int j, const int k);
	double calcDxy(Grid &g, const int i, const int j, const int k);
};
#endif