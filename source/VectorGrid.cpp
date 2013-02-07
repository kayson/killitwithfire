#include "VectorGrid.h"

VectorGrid::VectorGrid(const int x, const int y, const int z)
{
	xDim = x;
	yDim = y;
	zDim = z;

	grid = new Vector3**[xDim];
	for(int i = 0; i < xDim; i++)
	{
		grid[i] = new Vector3*[yDim];
		for(int j = 0; j < yDim; j++)
			grid[i][j] = new Vector3[zDim];
	}
}

VectorGrid::~VectorGrid()
{
	for(int i = 0; i < xDim; i++)
	{
		for(int j = 0; j < yDim; j++)
			delete [] grid[i][j];

		delete [] grid[i];
	}
}


void VectorGrid::setData(int x, int y, int z, const Vector3 &value)
{ 
	grid[x][y][z] = value; 
};
	
Vector3 & VectorGrid::operator()(const int x, const int y, const int z)
{
	if(x < 0 || y < 0 || z || 0) //Bör nog extrapolera här
		throw;
	if(x >= xDim || y >= yDim || z >= zDim) //Bör nog extrapolera här
		throw;

	return grid[x][y][z];
};

int VectorGrid::getDimX() const 
{ 
	return xDim; 
};

int VectorGrid::getDimY() const 
{ 
	return yDim; 
};

int VectorGrid::getDimZ() const 
{ 
	return zDim; 
};

double VectorGrid::getMax()
{
	double max = 0;
	for(int i = 0; i < xDim; i++)
	{
		for(int j = 0; j < yDim; j++)
		{
			for(int k = 0; k < zDim; k++)
			{
				double d = grid[i][j][k].norm();
				if (d > max)
				{
					max = d;
				}
			}
		}
	}

	return max;
}