#ifndef LEVELSET_H
#define LEVELSET_H

#include "../datatype.h"
#include "../Grid.h"

class LevelSet
{
public:
	LevelSet(){};
	~LevelSet(){};
	void fillLevelSet(data (*implicitFunction)(int x, int y, int z));
	void specifyRenderFunction(void (*renderFunction)());

	void draw() const;
	void printDistanceField();
private:
	Grid phi; //Signed distance field
	void (*render)();
};
#endif //LEVELSET_H