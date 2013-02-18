

#ifndef __FuidFire__AdvectEuler__
#define __FuidFire__AdvectEuler__

#include "Advect.h"
#include "../VelocityField.h"

class AdvectEuler : public Advect {
public:
    AdvectEuler(){};

    virtual void advect(VelocityField &v, GridField<double> *g, GridField<double> *ng, double dt);
	void integrateEuler(VelocityField &v, GridField<double> *g, GridField<double> *ng, double dt);
	double evaluate(VelocityField &v, GridField<double> &g, unsigned int i, unsigned int j, unsigned int k);

};
#endif /* defined(__FuidFire__AdvectEuler__) */
