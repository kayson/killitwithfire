#ifndef __FuidFire__VelocityField__
#define __FuidFire__VelocityField__

#include "Grid.h"
#include "Vector3.h"

class VelocityField{
private:
    Grid u,v,w;
public:
    VelocityField();

	double getMax();

    Vector3 getVelocityAtWorldCoordinate(Vector3 &pos);
};
#endif /* defined(__FuidFire__VelocityField__) */
