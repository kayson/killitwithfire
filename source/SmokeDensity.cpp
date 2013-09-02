#include "SmokeDensity.h"
#include "GridField.hpp"
#include "LevelSet.h"
#include "Vector3.h"
#include "MACGrid.h"
#include <iostream>

const double maxdens = 0.05;
const double mindens = 0.0;

SmokeDensity::SmokeDensity(unsigned int xdim,unsigned int ydim,unsigned int zdim, double size, const LevelSet &ls)
{
	grid = new GridField<double>(xdim, ydim, zdim, size, new ConstantValueExtrapolation<double>(0.0));
    copy = grid; //kopiera

	for(int i = 0; i < grid->xdim(); i++)
        for(int j = 0; j < grid->ydim(); j++)
            for(int k = 0; k < grid->zdim(); k++)
			{
				grid->setValueAtIndex(0.0, i, j, k);
			}

	resetCells(ls);
}

void SmokeDensity::resetCells(const LevelSet &ls)
{
	for(int i = 0; i < grid->xdim(); i++)
        for(int j = 0; j < grid->ydim(); j++)
            for(int k = 0; k < grid->zdim(); k++)
			{
				double x,y,z;
                grid->indexToWorld(i, j, k, x, y, z);
				if(ls.getCellType(x, y, z) == FUEL)
					grid->setValueAtIndex(maxdens, i, j, k);
				else
				{
					double d = grid->valueAtIndex(i, j, k);
					if(d < mindens)
						grid->setValueAtIndex(mindens, i, j, k);
					else if(d > maxdens)
						grid->setValueAtIndex(maxdens, i, j, k);
				}
			}
}



void SmokeDensity::advectDensityField(double dt, const MACGrid &m, const LevelSet &ls){
    for(int i = 0; i < grid->xdim(); i++)
        for(int j = 0; j < grid->ydim(); j++)
            for(int k = 0; k < grid->zdim(); k++){
                double x,y,z;
                grid->indexToWorld(i, j, k, x, y, z);
                Vector3 vel = m.velocityAtWorld(Vector3(x,y,z));
                double val = grid->valueAtWorld(x-dt*vel.x, y-dt*vel.y, z-dt*vel.z);

				assert(val >= mindens && val <= maxdens);

                copy->setValueAtIndex(val, i, j, k);
    }
    
    std::swap(grid, copy);

	resetCells(ls);
}

