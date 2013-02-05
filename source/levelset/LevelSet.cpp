#include "LevelSet.h"
#include <iostream>

LevelSet::LevelSet()
{

}

void LevelSet::fillLevelSet(data (*implicitFunction)(int, int, int))
{
	for(int i = 0; i < phi.getDimX(); i++)
		for(int j = 0; j < phi.getDimY(); j++)
			for(int k = 0; k < phi.getDimZ(); k++)
			{
				phi(i, j, k) = implicitFunction(i, j, k);
	}
}
void LevelSet::printDistanceField()
{
	for(int i = 0; i < phi.getDimX(); i++)
	{
		for(int j = 0; j < phi.getDimY(); j++)
		{
			for(int k = 0; k < phi.getDimZ(); k++)
			{
				std::cout << phi(i, j, k) << " ";
			}
			
		}
		std::cout << std::endl;
	}
}

void LevelSet::draw() const
{
	phi.draw();
}

arma::vec LevelSet::getCellVelocity(Grid<data> &u, Grid<data> &v, Grid<data> &w, int i, int j, int k)
{
	arma::vec tmp(3);
	tmp(0) = (u(i-1,j,k) + u(i+1,j,k))/4;
	tmp(1) = (v(i,j-1,k) + v(i,j+1,k))/4;
	tmp(2) = (w(i,j,k-1) + w(i,j,k+1))/4;
	
	return tmp;
}