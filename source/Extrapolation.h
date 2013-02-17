// Johan Noren
// Abstrakt bas-klass for olika extrapolerings-implementeringar

#ifndef EXTRAPOLATION_H
#define EXTRAPOLATION_H

#include "Discretization.h"

template<class T>
class Extrapolation{
public:
    virtual ~Extrapolation(){};
	virtual double extrapolate(GridField<double>& g, const int i, const int j, const int k, T &t)=0;
	virtual Vector3 extrapolate(GridField<Vector3>& g, const int i, const int j, const int k, T &t)=0;
};

#endif