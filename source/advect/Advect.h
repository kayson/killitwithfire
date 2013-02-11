
#ifndef __FuidFire__Advect__
#define __FuidFire__Advect__

#include "../VelocityField.h"
#include "../Discretization.h"

class Advect {
protected:
	Discretization *discretization;
public:
    virtual ~Advect(){};
    virtual void advect(VelocityField &v,Grid &g, double dt) = 0;
	virtual void setDiscretization(Discretization *discret){ discretization = discret;};
};
#endif /* defined(__FuidFire__Advect__) */
