// Johan Noren
#ifndef CENTRALDIFF_H
#define CENTRALDIFF_H
#include "Discretization.h"
#include "GhostGrid.h"

class CentralDiff : public Discretization{
public:
	double calcDxp(GridField<double> &g, const int i, const int j, const int k);
	double calcDyp(GridField<double> &g, const int i, const int j, const int k);
	double calcDzp(GridField<double> &g, const int i, const int j, const int k);

	double calcDxm(GridField<double> &g, const int i, const int j, const int k);
	double calcDym(GridField<double> &g, const int i, const int j, const int k);
	double calcDzm(GridField<double> &g, const int i, const int j, const int k);

	double calcDx(GridField<double> &g, const int i, const int j, const int k);
	double calcDy(GridField<double> &g, const int i, const int j, const int k);
	double calcDz(GridField<double> &g, const int i, const int j, const int k);

	double calcD2x(GridField<double> &g, const int i, const int j, const int k);
	double calcD2y(GridField<double> &g, const int i, const int j, const int k);
	double calcD2z(GridField<double> &g, const int i, const int j, const int k);

	double calcDyz(GridField<double> &g, const int i, const int j, const int k);
	double calcDxz(GridField<double> &g, const int i, const int j, const int k);
	double calcDxy(GridField<double> &g, const int i, const int j, const int k);

	double calcDx(GhostGridField<double> &g, const int i, const int j, const int k);
	double calcDy(GhostGridField<double> &g, const int i, const int j, const int k);
	double calcDz(GhostGridField<double> &g, const int i, const int j, const int k);

	double calcD2x(GhostGridField<double> &g, const int i, const int j, const int k);
	double calcD2y(GhostGridField<double> &g, const int i, const int j, const int k);
	double calcD2z(GhostGridField<double> &g, const int i, const int j, const int k);

	double calcDyz(GhostGridField<double> &g, const int i, const int j, const int k);
	double calcDxz(GhostGridField<double> &g, const int i, const int j, const int k);
	double calcDxy(GhostGridField<double> &g, const int i, const int j, const int k);


	Vector3 calcDx(GridField<Vector3> &g, const int i, const int j, const int k);
	Vector3 calcDy(GridField<Vector3> &g, const int i, const int j, const int k);
	Vector3 calcDz(GridField<Vector3> &g, const int i, const int j, const int k);

	Vector3 calcD2x(GridField<Vector3> &g, const int i, const int j, const int k);
	Vector3 calcD2y(GridField<Vector3> &g, const int i, const int j, const int k);
	Vector3 calcD2z(GridField<Vector3> &g, const int i, const int j, const int k);

	Vector3 calcDyz(GridField<Vector3> &g, const int i, const int j, const int k);
	Vector3 calcDxz(GridField<Vector3> &g, const int i, const int j, const int k);
	Vector3 calcDxy(GridField<Vector3> &g, const int i, const int j, const int k);
};

#endif