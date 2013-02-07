#include "LevelSet.h"
#include <iostream>

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