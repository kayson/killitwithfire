// Johan Noren
// Abstrakt bas-klass for olika extrapolerings-implementeringar

#ifndef EXTRAPOLATION_H
#define EXTRAPOLATION_H

template<class T> class GridField;

template<class T>
class Extrapolation{
public:
    virtual ~Extrapolation(){}
	virtual Extrapolation<T>* clone() const = 0;
	virtual T extrapolate(const GridField<T>& g, const int i, const int j, const int k) const = 0;
};

#endif