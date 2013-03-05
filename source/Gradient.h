#ifndef GRADIENT_H
#define GRADIENT_H

class Vector3;
class Discretization;
template<class T> class GridField;

class Gradient
{
public:
	Gradient();
	static Vector3 getGradient(GridField<double> &g, const int i, const int j, const int k, Discretization &d);
};
#endif