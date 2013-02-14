#ifndef CENTRALDIFF_H
#define CENTRALDIFF_H
#include "Discretization.h"

class CentralDiff : public Discretization{
public:
	CentralDiff();
	~CentralDiff();
	double calcDx(GridField<double> &g, const int i, const int j, const int k);
	double calcDy(GridField<double> &g, const int i, const int j, const int k);
	double calcDz(GridField<double> &g, const int i, const int j, const int k);

	double calcD2x(GridField<double> &g, const int i, const int j, const int k);
	double calcD2y(GridField<double> &g, const int i, const int j, const int k);
	double calcD2z(GridField<double> &g, const int i, const int j, const int k);

	double calcDyz(GridField<double> &g, const int i, const int j, const int k);
	double calcDxz(GridField<double> &g, const int i, const int j, const int k);
	double calcDxy(GridField<double> &g, const int i, const int j, const int k);
};

#endif