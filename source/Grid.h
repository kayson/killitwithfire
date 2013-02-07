#ifndef GRID_H
#define GRID_H

#include "datatype.h"
#include "Vector3.h"
#ifdef __APPLE__
#include "glfw.h"
#elif defined _WIN32 || defined _WIN64
#include <GL/glfw.h>
#endif

class Grid
{
public:
	Grid(const int x = GRID_DIM_X, const int y = GRID_DIM_Y, const int z = GRID_DIM_Z);

	~Grid();

	void draw() const;

	//Setters
	void setData(int x, int y, int z, data value);
	
	data& operator()(const int x, const int y, const int z);

	//Getters
	int getDimX() const;
	int getDimY() const;
	int getDimZ() const;

	data& getMax();

	Vector3 getCellVelocity(Grid &u, Grid &v, Grid &w, const int i, const int j, const int k);
	Vector3 getGridVelocity(Grid &u, Grid &v, Grid &w, const int i, const int j, const int k, DirectionEnums dir);

private:
	void drawLines2d() const;

	void drawFilled2d() const;

	data ***grid;
	int xDim, yDim, zDim;
};

#endif
