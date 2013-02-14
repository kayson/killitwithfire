#ifndef CENTRALDIFF_H
#define CENTRALDIFF_H
#include "Discretization.h"
#include "Extrapolation.h"
#include "ExtrapolateConstant.h"

class CentralDiff : public Discretization{
public:
	CentralDiff();
	~CentralDiff();
	data calcDx(Grid &g, const int i, const int j, const int k);
	data calcDy(Grid &g, const int i, const int j, const int k);
	data calcDz(Grid &g, const int i, const int j, const int k);

	data calcD2x(Grid &g, const int i, const int j, const int k);
	data calcD2y(Grid &g, const int i, const int j, const int k);
	data calcD2z(Grid &g, const int i, const int j, const int k);

	data calcDyz(Grid &g, const int i, const int j, const int k);
	data calcDxz(Grid &g, const int i, const int j, const int k);
	data calcDxy(Grid &g, const int i, const int j, const int k);
};

#endif