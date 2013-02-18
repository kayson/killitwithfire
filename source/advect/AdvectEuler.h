

#ifndef __FuidFire__AdvectEuler__
#define __FuidFire__AdvectEuler__

#include "Advect.h"
#include "../VelocityField.h"

class AdvectEuler : public Advect {
public:
    AdvectEuler(){};
    virtual void advect(MACGrid &u, GridField<double> *g,GridField<double> *temp, double dt);
	void integrateEuler(MACGrid &u, GridField<double> &g, double dt);
	double evaluate(MACGrid &u, GridField<double> &g, unsigned int i, unsigned int j, unsigned int k);
};
#endif /* defined(__FuidFire__AdvectEuler__) */
