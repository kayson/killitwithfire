#ifndef CENTRALDIFF_H
#define CENTRALDIFF_H
#include "Discretization.h"

class CentralDiff : public Discretization{
public:
	CentralDiff();
	~CentralDiff();
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