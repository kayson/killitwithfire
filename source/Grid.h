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

private:
	data ***grid;
	int xDim, yDim, zDim;
};

#endif
