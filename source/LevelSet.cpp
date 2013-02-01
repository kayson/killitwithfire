#include "LevelSet.h"

LevelSet::LevelSet()
{

}

void LevelSet::fillLevelSet(data (*implicitFunction)(int, int, int))
{
	for(int x = 0; x < phi.getDimX(); x++)
		for(int y = 0; y < phi.getDimY(); y++)
			for(int z = 0; z < phi.getDimZ(); z++)
			{
				phi(x, y, z) = implicitFunction(x,y,z);
	}

}