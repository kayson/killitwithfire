#ifndef LEVELSET_H
#define LEVELSET_H

#include "../Vector3.h"
#include "../GridField.hpp"
#include "../MACGrid.h"

class LevelSet
{
public:
	LevelSet(){
		grid  = new GridField<double>(1000,1000,1000);
		gridCopy = new GridField<double>(1000, 1000, 1000,10);
    };
	LevelSet(int xDim, int yDim, int zDim, double size){
		grid  = new GridField<double>(xDim,yDim,zDim,size);
		gridCopy = new GridField<double>(xDim,yDim,zDim,size);
	};
	~LevelSet(){};
	void fillLevelSet(double (*implicitFunction)(int x, int y, int z));
	void specifyRenderFunction(void (*renderFunction)());

	void draw() const;
	void printDistanceField();
	double getCurvature(const int i, const int j, const int k);
	Vector3 getNormal(const int i, const int j, const int k);
	Vector3 getNormal(const double w_x, const double w_y, const double w_z);

	void reinitialize();

	GridField<double> *grid; //Signed distance field
	GridField<double> *gridCopy;

    
    Vector3 getVelocity(MACGrid &g, int i, const int j, const int k);
private:
	
	void (*render)();
};
#endif //LEVELSET_H