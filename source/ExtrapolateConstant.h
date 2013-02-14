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
		if(typeid(d) == typeid(CentralDiff))
		{
			// Första-derivata
			if(i <= 0)
				return (double)(g(i+1, j, k) - g(i, j, k))/(FirePresets::dx*2.0);
			if(i >= g.getDimX()-1)
				return (double)(g(i, j, k) - g(i-1, j, k))/(FirePresets::dx*2.0);
			if(j <= 0)
				return (double)(g(i, j+1, k) - g(i, j, k))/(FirePresets::dx*2.0);  
			if(j >= g.getDimY()-1)
				return (double)(g(i, j, k) - g(i, j-1, k))/(FirePresets::dx*2.0);
			if(k <= 0)
				return (double)(g(i, j, k+1) - g(i, j, k))/(FirePresets::dx*2.0);
			if(k >= g.getDimZ()-1)
				return (double)(g(i, j, k) - g(i, j, k-1))/(FirePresets::dx*2.0);

			// Andra-derivata
		}

		return 0;
	}
};
#endif