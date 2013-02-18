#ifndef VECTORGRID_H
#define VECTORGRID_H

#include "Vector3.h"

class VectorGrid{
    
private:
    void initGrid();
    
public:
	VectorGrid(const int x = 1, const int y = 1, const int z = 1);

	~VectorGrid();

	void draw() const;

	//Setters
	void setData(const int x, const int y, const int z, const Vector3 &value);
	
	Vector3 & operator()(const int x, const int y, const int z);
    
    void operator=(const VectorGrid &v);

	//Getters
	int getDimX() const;
	int getDimY() const;
	int getDimZ() const;

	double getMax();

private:
	Vector3 ***grid;
	int xDim, yDim, zDim;
};

#endif
