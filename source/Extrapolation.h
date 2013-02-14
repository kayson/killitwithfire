#ifndef EXTRAPOLATION_H
#define EXTRAPOLATION_H

//#include "Grid.h"
#include "Discretization.h"

template<class T>
class Extrapolation{
public:
	virtual double extrapolate(Grid& g, const int i, const int j, const int k, T &t)=0;
};

#endif