#ifndef GRID_H
#define GRID_H

#include "datatype.h"
#include <vector>
//Denna klass bör nog vara en template klass, då griden skall innehålla densitet, tryck, hastighetsfält, mm...

class Grid
{
public:
	Grid(const int x = GRID_DIM_X, const int y = GRID_DIM_Y, const int z = GRID_DIM_Z);
	~Grid() 
	{ 
		//Fixa destruktor 
	}

	void draw() const;

	//Setters
	void setData(int x, int y, int z, data value){ grid[x][y][z] = value; };
	
	data& operator()(const int x, const int y, const int z)
	{
		if(x < xDim && y < yDim && z < zDim)
			return grid[x][y][z];
	};

	//Getters
	int getDimX() const { return xDim; };
	int getDimY() const { return yDim; };
	int getDimZ() const { return zDim; };

private:
	void drawLines2d() const;
	void drawFilled2d() const;

	data ***grid;
	int xDim, yDim, zDim;
};

#endif
