#ifndef CLOSESTVALUEEXTRAPOLATION_H
#define CLOSESTVALUEEXTRAPOLATION_H

#include "Extrapolation.h"
#include "GridField.h"

template<class T>
class ClosestValueExtrapolation  : public Extrapolation<T>
{
public:
    virtual ~ClosestValueExtrapolation(){}

	virtual Extrapolation<T>* clone() const
	{
		return new ClosestValueExtrapolation<T>();
	}

	virtual T doExtrapolation(const GridField<T>& g, int i, int j, int k) const{
		// find closest point by cityblock distance
		if(i < 0)
			i = 0;
		else if(i >= g.xdim())
			i = g.xdim()-1;

		if(j < 0)
			j = 0;
		else if(j >= g.ydim())
			j = g.ydim()-1;

		if(k < 0)
			k = 0;
		else if(k >= g.zdim())
			k = g.zdim()-1;
        
        return g.valueAtIndex(i, j, k);
	}
};

#endif