/*
	Bas-klass f�r olika diskretiseringsmetoder. Virtuella metoder
	skall �verlagras av sub-klasser
	Johan Noren
*/

#ifndef DISCRETIZATION_H
#define DISCRETIZATION_H

class Vector3;
template<class T> class GridField;

class Discretization{
public:
	//Discretization();
	virtual ~Discretization(){}

	// Dessa skall och m�ste �verlagras av subklasser
	virtual double calcDxp(GridField<double> &g, const int i, const int j, const int k)=0;
	virtual double calcDyp(GridField<double> &g, const int i, const int j, const int k)=0;
	virtual double calcDzp(GridField<double> &g, const int i, const int j, const int k)=0;

	virtual double calcDxm(GridField<double> &g, const int i, const int j, const int k)=0;
	virtual double calcDym(GridField<double> &g, const int i, const int j, const int k)=0;
	virtual double calcDzm(GridField<double> &g, const int i, const int j, const int k)=0;

	virtual double calcDx(GridField<double> &g, const int i, const int j, const int k)=0;
	virtual double calcDy(GridField<double> &g, const int i, const int j, const int k)=0;
	virtual double calcDz(GridField<double> &g, const int i, const int j, const int k)=0;

	virtual double calcD2x(GridField<double> &g, const int i, const int j, const int k)=0;
	virtual double calcD2y(GridField<double> &g, const int i, const int j, const int k)=0;
	virtual double calcD2z(GridField<double> &g, const int i, const int j, const int k)=0;

	virtual double calcDyz(GridField<double> &g, const int i, const int j, const int k)=0;
	virtual double calcDxz(GridField<double> &g, const int i, const int j, const int k)=0;
	virtual double calcDxy(GridField<double> &g, const int i, const int j, const int k)=0;
	
	virtual  Vector3 calcDx(GridField<Vector3> &g, const int i, const int j, const int k)=0;
	virtual  Vector3 calcDy(GridField<Vector3> &g, const int i, const int j, const int k)=0;
	virtual  Vector3 calcDz(GridField<Vector3> &g, const int i, const int j, const int k)=0;
			 
	virtual  Vector3 calcD2x(GridField<Vector3> &g, const int i, const int j, const int k)=0;
	virtual  Vector3 calcD2y(GridField<Vector3> &g, const int i, const int j, const int k)=0;
	virtual  Vector3 calcD2z(GridField<Vector3> &g, const int i, const int j, const int k)=0;
			 
	virtual  Vector3 calcDyz(GridField<Vector3> &g, const int i, const int j, const int k)=0;
	virtual  Vector3 calcDxz(GridField<Vector3> &g, const int i, const int j, const int k)=0;
	virtual  Vector3 calcDxy(GridField<Vector3> &g, const int i, const int j, const int k)=0;
};
#endif