#ifndef LEVELSET_H
#define LEVELSET_H

#include "../datatype.h"
#include "../Grid.h"

#ifdef __APPLE__
#include "armadillo.h"
#elif defined _WIN32 || defined _WIN64
#include "armadillo"
#endif
class LevelSet
{
public:
	LevelSet();
	~LevelSet(){};
	void fillLevelSet(data (*implicitFunction)(int x, int y, int z));
	void specifyRenderFunction(void (*renderFunction)());
	//Grid<data> & getVelocityGrid();

	arma::vec getCellVelocity(Grid<data> &u, Grid<data> &v, Grid<data> &w, const int i, const int j, const int k);

	void draw() const;
	void printDistanceField();
private:
	Grid<data> phi; //Signed distance field
	void (*render)();
};
#endif //LEVELSET_H