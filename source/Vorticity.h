// Johan Norén, 2013-02-26
#ifndef VORTICITY_H
#define VORTICITY_H

class MACGrid;
template<class T> class GridField;
class Vector3;
class Vorticity{
public:
	static void addVorticity(const MACGrid &u, GridField<Vector3> &forces, const double epsilon, const double dx, const int dimx, const int dimy, const int dimz);
};
#endif