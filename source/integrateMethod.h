#ifndef INTEGRATE_H
#define INTEGRATE_H
#include "GridField.h"
#include "MACGrid.h"

#ifdef __APPLE__
#include "firePresets.h"
#elif defined _WIN32 || defined _WIN64
#include "presets\firePresets.h"
#endif


class IntegrateMethod
{
public:
	virtual ~IntegrateMethod(){};

	virtual void calculateIntegral(GridField<double> &g, double dt, double (*Evaluate)(GridField<double> &g, int i, int j, int k)) = 0;

	virtual void calculateIntegral(MACGrid &u, GridField<double> &g, double dt, double (*Evaluate)(MACGrid &u, GridField<double> &g, int i, int j, int k)) = 0;

};

#endif