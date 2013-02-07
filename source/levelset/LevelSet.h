#ifndef LEVELSET_H
#define LEVELSET_H

#include "../datatype.h"
#include "../Grid.h"
#include "../Vector3.h"

class LevelSet
{
public:
	LevelSet(){};
	~LevelSet(){};
	void fillLevelSet(data (*implicitFunction)(int x, int y, int z));
	void specifyRenderFunction(void (*renderFunction)());

	void draw() const;
	void printDistanceField();
	double getCurvature(const int i, const int j, const int k);
	Vector3 getNormal(const int i, const int j, const int k);
private:
	Grid phi; //Signed distance field
	void (*render)();
};
#endif //LEVELSET_H