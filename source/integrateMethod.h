#ifndef INTEGRATE_H
#define INTEGRATE_H

template<class T> class GridField;
class MACGrid;

class IntegrateMethod
{
public:
	virtual ~IntegrateMethod(){};

	virtual void calculateIntegral(GridField<double> **grid, GridField<double> **gridCopy, double dt, double (*Evaluate)(GridField<double> &g, int i, int j, int k)) = 0;

	virtual void calculateIntegral(MACGrid &u, GridField<double> **grid, GridField<double> **gridCopy, double dt, double (*Evaluate)(MACGrid &u, GridField<double> &g, int i, int j, int k)) = 0;

};

#endif