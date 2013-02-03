#ifndef LEVELSET_H
#define LEVELSET_H

//#include "datatype.h"
#include "Grid.h"

class LevelSet
{
public:
	LevelSet();
	~LevelSet(){};
	void fillLevelSet(data (*implicitFunction)(int x, int y, int z));
	void specifyRenderFunction(void (*renderFunction)());
	Grid<data> & getVelocityGrid();

	void draw() const;
	void printVelocityGrid();
private:
	Grid<data> u; //Velocity grid, BÖR VARA EN MACGRID!!!
	Grid<data> phi; //Levelset, byta namn kanske?
	void (*render)();
};
#endif //LEVELSET_H