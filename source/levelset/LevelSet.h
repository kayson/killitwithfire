#ifndef LEVELSET_H
#define LEVELSET_H

#include "../datatype.h"
#include "../Vector3.h"
#include "../GridField.hpp"

class LevelSet
{
public:
	LevelSet(){
		phi  = new GridField<double>(1000,1000,1000);
		temp = new GridField<double>(1000, 1000, 1000);
    };
	LevelSet(int xDim, int yDim, int zDim){
		phi  = new GridField<double>(xDim,yDim,zDim);
		temp = new GridField<double>(xDim,yDim,zDim);
	};
	~LevelSet(){};
	void fillLevelSet(double (*implicitFunction)(int x, int y, int z));
	void specifyRenderFunction(void (*renderFunction)());

	void draw() const;
	void printDistanceField();
	double getCurvature(const int i, const int j, const int k);
	Vector3 getNormal(const int i, const int j, const int k);

	void reinitialize();

	GridField<double> *phi; //Signed distance field
	GridField<double> *temp;

private:
	
	void (*render)();
};
#endif //LEVELSET_H