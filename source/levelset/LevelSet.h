#ifndef LEVELSET_H
#define LEVELSET_H

#include "../datatype.h"
#include "../Vector3.h"
#include "../GridField.hpp"

class LevelSet
{
public:
	LevelSet(){};
	LevelSet(int xDim, int yDim, int zDim){
		phi = *(new GridField(xDim, yDim, zDim));
	};
	~LevelSet(){};
	void fillLevelSet(double (*implicitFunction)(int x, int y, int z));
	void specifyRenderFunction(void (*renderFunction)());

	void draw() const;
	void printDistanceField();
	double getCurvature(const int i, const int j, const int k);
	Vector3 getNormal(const int i, const int j, const int k);

	GridField<double> phi; //Signed distance field
private:
	
	void (*render)();
};
#endif //LEVELSET_H