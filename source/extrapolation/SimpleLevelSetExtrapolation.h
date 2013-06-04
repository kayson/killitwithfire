//Axel väldigt simpel approximering av ett levelset utanför griden

#ifndef SIMPLELEVELSETEXTRAPOLATION_H
#define SIMPLELEVELSETEXTRAPOLATION_H

#include "Extrapolation.h"
#include "GridField.h"
#include "firePresets.h"

class SimpleLevelSetExtrapolation  : public Extrapolation<double>
{
public:
    virtual ~SimpleLevelSetExtrapolation(){}

	virtual Extrapolation<double>* clone() const
	{
		return new SimpleLevelSetExtrapolation();
	}

	virtual double doExtrapolation(const GridField<double>& g, int i, int j, int k) const
	{
		double x1 = i;
		double y1 = j;
		double z1 = k;

		//Hitta närmaste punkt via cityblock
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

		double x0 = i;
		double y0 = j;
		double z0 = k;

		return g.valueAtIndex(i, j, k) - g.dx()*std::sqrt( std::pow(x1-x0, 2.0) + std::pow(y1-y0, 2.0) + std::pow(z1-z0, 2.0) );
	}
};

#endif