// Johan Noren
// Denna class genomfor konstant extrapolering. Om en operator forsoker
// hamta data fran utanfor griden extrapoleras vardet till den
// narmaste grid-cellen.

#ifndef EXTRAPOLATECONSTANT_H
#define EXTRAPOLATECONSTANT_H

#include "Extrapolation.h"
#include "CentralDiff.h"
#include "UpwindDiff.h"
class ExtrapolateConstant : public Extrapolation<Discretization>{
public:
	double extrapolate(GridField<double>& g, const int i, const int j, const int k, Discretization &d)
	{
		if(typeid(d) == typeid(UpwindDiff))
		{
			if(i <= 0 || i >= g.xdim()-1)
				return 0;
			if(j <= 0 || j >= g.ydim()-1)
				return 0;
			if(k <= 0 || k >= g.zdim()-1)
				return 0;
		}
		if(typeid(d) == typeid(CentralDiff))
		{
			// F�rsta-derivata
			if(i <= 0)
				return (double)(g(i+1, j, k) - g(i, j, k))/(FirePresets::dx*2.0);
			if(i >= g.xdim()-1)
				return (double)(g(i, j, k) - g(i-1, j, k))/(FirePresets::dx*2.0);
			if(j <= 0)
				return (double)(g(i, j+1, k) - g(i, j, k))/(FirePresets::dx*2.0);  
			if(j >= g.ydim()-1)
				return (double)(g(i, j, k) - g(i, j-1, k))/(FirePresets::dx*2.0);
			if(k <= 0)
				return (double)(g(i, j, k+1) - g(i, j, k))/(FirePresets::dx*2.0);
			if(k >= g.zdim()-1)
				return (double)(g(i, j, k) - g(i, j, k-1))/(FirePresets::dx*2.0);

			// Andra-derivata
		}

		return 0;
	}

	Vector3 extrapolate(GridField<Vector3>& g, const int i, const int j, const int k, Discretization &d)
	{
		if(typeid(d) == typeid(UpwindDiff))
		{
			if(i <= 0 || i >= g.xdim()-1)
				return Vector3();
			if(j <= 0 || j >= g.ydim()-1)
				return Vector3();
			if(k <= 0 || k >= g.zdim()-1)
				return Vector3();
		}
		if(typeid(d) == typeid(CentralDiff))
		{
			// F�rsta-derivata
			if(i <= 0)
				return (g(i+1, j, k) - g(i, j, k))/(FirePresets::dx*2.0);
			if(i >= g.xdim()-1)
				return (g(i, j, k) - g(i-1, j, k))/(FirePresets::dx*2.0);
			if(j <= 0)
				return (g(i, j+1, k) - g(i, j, k))/(FirePresets::dx*2.0);  
			if(j >= g.ydim()-1)
				return (g(i, j, k) - g(i, j-1, k))/(FirePresets::dx*2.0);
			if(k <= 0)
				return (g(i, j, k+1) - g(i, j, k))/(FirePresets::dx*2.0);
			if(k >= g.zdim()-1)
				return (g(i, j, k) - g(i, j, k-1))/(FirePresets::dx*2.0);

			// Andra-derivata
		}

		return Vector3();
	}
};
#endif