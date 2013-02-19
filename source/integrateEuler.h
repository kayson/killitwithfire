#ifndef INTEGRATE_EULER_H
#define INTEGRATE_EULER_H

#include "integrateMethod.h"

class IntegrateEuler : public IntegrateMethod
{
public:
	IntegrateEuler(){};

	virtual void calculateIntegral(GridField<double> &g, double dt, double (*Evaluate)(GridField<double> &g, int i, int j, int k))
	{
		for(int i = 0; i < g.xdim(); i++)
		{
			for(int j = 0; j < g.ydim(); j++)
			{
				for(int k = 0; k < g.zdim(); k++)
				{
					double ddt = Evaluate(g, i, j, k);
					g.setValueAtIndex(g.valueAtIndex(i,j,k) + ddt * dt, i, j, k);
				}
			}
		}
	};
	virtual void calculateIntegral(MACGrid &u, GridField<double> &g, double dt, double (*Evaluate)(MACGrid &u, GridField<double> &g, int i, int j, int k))
	{
		for(int i = 0; i < g.xdim(); i++)
		{
			for(int j = 0; j < g.ydim(); j++)
			{
				for(int k = 0; k < g.zdim(); k++)
				{
					double ddt = Evaluate(u, g, i, j, k);
					g.setValueAtIndex(g.valueAtIndex(i,j,k) + ddt * dt, i, j, k);
				}
			}
		}
	};

};

#endif