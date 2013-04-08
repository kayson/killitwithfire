//Axel väldigt simpel approximering av ett levelset utanför griden

#ifndef CLOSESTVALUEEXTRAPOLATION_H
#define CLOSESTVALUEEXTRAPOLATION_H

#include "Extrapolation.h"
#include "firePresetsTwoDimension.h"

class ClosestValueExtrapolation 
{
public:
    virtual ~ClosestValueExtrapolation(){}

	virtual double extrapolate(GridField<double>& g, const int i, const int j, const int k) 
	{
		int a = i;
		int b = j;
		int c = k;

		//Hitta närmaste punkt via cityblock
		if(a < 0)
			a = 0;
		else if(a >= g.xdim())
			a = g.xdim()-1;

		if(b < 0)
			b = 0;
		else if(b >= g.ydim())
			b = g.ydim()-1;

		if(c < 0)
			c = 0;
		else if(c >= g.zdim())
			c = g.zdim()-1;

		return g.valueAtIndex(a, b, c);
	}

	virtual Vector3 extrapolate(GridField<Vector3>& g, const int i, const int j, const int k)
	{
		int a = i;
		int b = j;
		int c = k;

		//Hitta närmaste punkt via cityblock
		if(a < 0)
			a = 0;
		else if(a >= g.xdim())
			a = g.xdim()-1;

		if(b < 0)
			b = 0;
		else if(b >= g.ydim())
			b = g.ydim()-1;

		if(c < 0)
			c = 0;
		else if(c >= g.zdim())
			c = g.zdim()-1;

		return g.valueAtIndex(a, b, c);
	}
};

#endif