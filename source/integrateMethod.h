#ifndef INTEGRATE_H
#define INTEGRATE_H
#include "presets\firePresets.h"
#include "GridField.h"
#include "MACGrid.h"



class IntegrateMethod
{
public:
	virtual ~IntegrateMethod(){};

	virtual void calculateIntegral(GridField<double> &g, double dt, double (*Evaluate)(GridField<double> &g, int i, int j, int k)) = 0;

	virtual void calculateIntegral(MACGrid &u, GridField<double> &g, double dt, double (*Evaluate)(MACGrid &u, GridField<double> &g, int i, int j, int k)) = 0;

};

#endif