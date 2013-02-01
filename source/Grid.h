#ifndef GRID_H
#define GRID_H

#include "datatype.h"
#include <vector>

//Denna klass bör nog vara en template klass, då griden skall innehålla densitet, tryck, hastighetsfält, mm...

class Grid
{
public:
	Grid(const int x = 10, const int y = 10, const int z = 10);
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
	data ***grid;
	int xDim, yDim, zDim;
};

#endif
