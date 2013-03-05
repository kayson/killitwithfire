// Johan Noren
// Abstrakt bas-klass for olika extrapolerings-implementeringar

#ifndef EXTRAPOLATION_H
#define EXTRAPOLATION_H

template<class T> class GridField;
class Vector3;

//template<class T> kanske är trevligt... men då funkar inte sqrt osv?
class Extrapolation{
public:
    virtual ~Extrapolation(){};
	virtual double extrapolate(GridField<double>& g, const int i, const int j, const int k)=0;
	virtual Vector3 extrapolate(GridField<Vector3>& g, const int i, const int j, const int k)=0;
};

#endif