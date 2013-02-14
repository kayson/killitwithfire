#ifndef EXTRAPOLATECONSTANT_H
#define EXTRAPOLATECONSTANT_H

#include "Extrapolation.h"
#include "CentralDiff.h"
#include "UpwindDiff.h"
class ExtrapolateConstant : public Extrapolation<Discretization>{
public:
	double extrapolate(Grid& g, const int i, const int j, const int k, Discretization &d)
	{
		if(typeid(d) == typeid(UpwindDiff))
		{
			if(i <= 0 || i >= g.getDimX()-1)
				return 0;
			if(j <= 0 || j >= g.getDimY()-1)
				return 0;
			if(k <= 0 || k >= g.getDimZ()-1)
				return 0;
		}
		/*if(typeid(d) == typeid(CentralDiff))
		{
		}*/

		return 0;
	}
};
#endif