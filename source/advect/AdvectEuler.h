

#ifndef __FuidFire__AdvectEuler__
#define __FuidFire__AdvectEuler__

#include "Advect.h"
#include "../VelocityField.h"

class AdvectEuler : public Advect {
public:
    AdvectEuler(){};
    virtual void advect(VelocityField &u, GridField<double> &g, double dt);
	void integrateEuler(VelocityField &u, GridField<double> &g, double dt);
	double evaluate(VelocityField &u, GridField<double> &g, unsigned int i, unsigned int j, unsigned int k);
};
#endif /* defined(__FuidFire__AdvectEuler__) */
