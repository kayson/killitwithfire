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
			// Gör boundary-kontroll här och sedan konstant
			// extrapolering enl. Upwind
		}
		/*if(typeid(d) == typeid(CentralDiff))
		{
		}*/

		return 0;
	}
};
#endif