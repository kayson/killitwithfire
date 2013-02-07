#ifndef __FuidFire__VelocityField__
#define __FuidFire__VelocityField__

#include "Grid.h"
#include "Vector3.h"

class VelocityField{
private:
    Grid u,v,w;
public:
    VelocityField();
	data getMax();
    Vector3 getPositionAtFace(int i ,int j, int k, DirectionEnums direction);
    Vector3 getVelocityAtWorldCoordinate(Vector3 &pos);

    friend class Fire;
	double findMaximumVelocity(){return 1.4;};
};
#endif /* defined(__FuidFire__VelocityField__) */
