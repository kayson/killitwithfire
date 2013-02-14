#ifndef __FuidFire__VelocityField__
#define __FuidFire__VelocityField__

#include "Grid.h"
#include "Vector3.h"
#include "VectorGrid.h"

class VelocityField{
private:
    Grid u,v,w;
	VectorGrid centerVelocities;
	void initVelocityField();
	int xDim, yDim, zDim;
public:
    VelocityField();
    VelocityField(const VelocityField &v);
	VelocityField(int xDim, int yDim, int zDim){
		this->xDim = xDim; this->yDim = yDim; this->zDim = zDim;
		centerVelocities = *new VectorGrid(xDim, yDim, zDim);
		initVelocityField();
	};
    
    void operator=(const VelocityField &v);
    
	VectorGrid *getCenterVel(){ return &centerVelocities;};
	double getMax();
    Vector3 getPositionAtFace(int i ,int j, int k, DirectionEnums direction);
    Vector3 getVelocityAtCoordinate(Vector3 &pos);
	Grid *getGridU() { return &u; }
	Grid *getGridV() { return &v; }
	Grid *getGridW() { return &w; }
	void setCenterVel(Vector3 &v, const int i, const int j, const int k);

        
    friend class Fire;
	double findMaximumVelocity(){return 1.4;};
};
#endif /* defined(__FuidFire__VelocityField__) */
