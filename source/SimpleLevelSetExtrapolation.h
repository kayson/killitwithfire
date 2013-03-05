//Axel väldigt simpel approximering av ett levelset utanför griden

#ifndef SIMPLELEVELSETEXTRAPOLATION_H
#define SIMPLELEVELSETEXTRAPOLATION_H

template<class T> class GridField;
class Vector3;
//#include "Extrapolation.h"

#include <cmath>

class SimpleLevelSetExtrapolation //: public Extrapolation
{
public:
    virtual ~SimpleLevelSetExtrapolation();

	virtual double extrapolate(GridField<double>& g, const int i, const int j, const int k);

	virtual Vector3 extrapolate(GridField<Vector3>& g, const int i, const int j, const int k);
};

#endif