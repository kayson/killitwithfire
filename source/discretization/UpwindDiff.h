#ifndef UPWINDDIFF_H
#define UPWINDDIFF_H
#include "Discretization.h"

class UpwindDiff : public Discretization{
protected:
	GridField<Vector3> *_w;

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

	Vector3 calcDx(GridField<Vector3> &g, const int i, const int j, const int k);
	Vector3 calcDy(GridField<Vector3> &g, const int i, const int j, const int k);
	Vector3 calcDz(GridField<Vector3> &g, const int i, const int j, const int k);

	Vector3 calcD2x(GridField<Vector3> &g, const int i, const int j, const int k);
	Vector3 calcD2y(GridField<Vector3> &g, const int i, const int j, const int k);
	Vector3 calcD2z(GridField<Vector3> &g, const int i, const int j, const int k);

	Vector3 calcDyz(GridField<Vector3> &g, const int i, const int j, const int k);
	Vector3 calcDxz(GridField<Vector3> &g, const int i, const int j, const int k);
	Vector3 calcDxy(GridField<Vector3> &g, const int i, const int j, const int k);

	virtual void setVelocityField(GridField<Vector3> &w) { _w = &w; }
};
#endif