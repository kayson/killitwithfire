#ifndef UPWINDDIFF_H
#define UPWINDDIFF_H
#include "Discretization.h"

class UpwindDiff : public Discretization{
public:
	UpwindDiff();
	data calcDx(const Grid &g, const int i, const int j, const int k);
	data calcDy(const Grid &g, const int i, const int j, const int k);
	data calcDz(const Grid &g, const int i, const int j, const int k);

	data calcD2x(const Grid &g, const int i, const int j, const int k);
	data calcD2y(const Grid &g, const int i, const int j, const int k);
	data calcD2z(const Grid &g, const int i, const int j, const int k);

	data calcDyz(const Grid &g, const int i, const int j, const int k);
	data calcDxz(const Grid &g, const int i, const int j, const int k);
	data calcDxy(const Grid &g, const int i, const int j, const int k);
};
#endif