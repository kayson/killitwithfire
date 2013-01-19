#ifndef GRID_H
#define GRID_H

#include "datatype.h"
#include <vector>

//Denna klass b�r nog vara en template klass, d� griden skall inneh�lla densitet, tryck, hastighetsf�lt, mm...
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
