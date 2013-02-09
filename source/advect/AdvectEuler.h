

#ifndef __FuidFire__AdvectEuler__
#define __FuidFire__AdvectEuler__

#include "Advect.h"
#include "../VelocityField.h"

class AdvectEuler : public Advect {
public:
    AdvectEuler(){};
    virtual void advect(VelocityField &v, Grid &g, double dt);
	void integrateEuler(VelocityField &v, Grid &g, double dt);
	double evaluate(VelocityField &v, Grid &g, unsigned int i, unsigned int j, unsigned int k);
};
#endif /* defined(__FuidFire__AdvectEuler__) */
