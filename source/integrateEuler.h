#ifndef INTEGRATE_EULER_H
#define INTEGRATE_EULER_H

#include "integrateMethod.h"

class IntegrateEuler : public IntegrateMethod
{
public:
	IntegrateEuler(){}

	virtual void calculateIntegral(GridField<double> **grid, GridField<double> **gridCopy, double dt, double (*Evaluate)(GridField<double> &grid, int i, int j, int k))
	{
		for(int i = 0; i < (*grid)->xdim(); i++)
		{
			for(int j = 0; j < (*grid)->ydim(); j++)
			{
				for(int k = 0; k < (*grid)->zdim(); k++)
				{
					double ddt = Evaluate(**grid, i, j, k);
					(*gridCopy)->setValueAtIndex((*grid)->valueAtIndex(i,j,k) + ddt * dt, i, j, k);
				}
			}
		}
		GridField<double> *temp = *gridCopy;
		*gridCopy = *grid;
		*grid = temp;
	}
	virtual void calculateIntegral(MACGrid &u, GridField<double> **grid,GridField<double> **gridCopy, double dt, double (*Evaluate)(MACGrid &u, GridField<double> &g, int i, int j, int k))
	{
		for(int i = 0; i < (*grid)->xdim(); i++)
		{
			for(int j = 0; j < (*grid)->ydim(); j++)
			{
				for(int k = 0; k < (*grid)->zdim(); k++)
				{
					double ddt = Evaluate(u, **grid, i, j, k);
					(*gridCopy)->setValueAtIndex((*grid)->valueAtIndex(i,j,k) + ddt * dt, i, j, k);
				}
			}
		}
		GridField<double> *temp = *gridCopy;
		*gridCopy = *grid;
		*grid = temp;
	}

};

#endif