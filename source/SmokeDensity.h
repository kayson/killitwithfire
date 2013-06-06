#ifndef SMOKEDENSITY_H
#define SMOKEDENSITY_H

#include "helper.h"

class Vector3;
template<class T> class GridField;
class MACGrid;
class LevelSet;



class SmokeDensity{

public:
	SmokeDensity(unsigned int xdim,unsigned int ydim,unsigned int zdim, double size, const LevelSet &ls);

    void advectDensityField(double dt, const MACGrid &m, const LevelSet &ls);

	GridField<double> *grid;
    GridField<double> *copy;
protected:
	SmokeDensity() {}
	void resetCells(const LevelSet &ls);
};

#endif //TEMPERATURE_H
