#ifndef LEVELSET_H
#define LEVELSET_H

#include "../datatype.h"
#include "../Grid.h"
#include "armadillo"

class LevelSet
{
public:
	LevelSet();
	~LevelSet(){};
	void fillLevelSet(data (*implicitFunction)(int x, int y, int z));
	void specifyRenderFunction(void (*renderFunction)());
	//Grid<data> & getVelocityGrid();

	arma::vec getCellVelocity(Grid<data> &u, Grid<data> &v, Grid<data> &w, int i, int j, int k);

	void draw() const;
	void printDistanceField();
private:
	Grid<data> phi; //Signed distance field
	void (*render)();
};
#endif //LEVELSET_H