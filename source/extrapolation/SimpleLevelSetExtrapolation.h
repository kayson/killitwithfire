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

	virtual double extrapolate(const GridField<double>& g, const int i, const int j, const int k) const
	{
		double x1 = double(i);
		double y1 = double(j);
		double z1 = double(k);

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

		double x0 = double(a);
		double y0 = double(b);
		double z0 = double(c);

		return g.valueAtIndex(a, b, c) - FirePresets::dx*std::sqrt(std::pow(x1-x0, 2.0) + std::pow(y1-y0, 2.0) + std::pow(y1-y0, 2.0));
	}
};

#endif