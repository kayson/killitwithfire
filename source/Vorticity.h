// Johan Nor�n, 2013-02-26
#ifndef VORTICITY_H
#define VORTICITY_H

class MACGrid;
template<class T> class GridField;
class Vector3;
class Vorticity{
	// 1.	Skapa ett Gridfield ohm genom nabmla x u (dvs. curl)
	// 2.	Skapa N genom N = (gradienten av normen av ohm)/(gradient av t�ljaren)
	//		N kommer d� peka fr�n l�g-vorticity-omr�den till omr�den med
	//		h�g vorticity.
	// 3.	G�r (N x ohm)*dx*epsilon och spara i GridField-referensen forces.
public:
	static void addVorticity(const MACGrid &u, GridField<Vector3> &forces, const double epsilon, const double dx, const int dimx, const int dimy, const int dimz);
};
#endif