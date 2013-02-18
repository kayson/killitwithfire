
#ifndef __FuidFire__Advect__
#define __FuidFire__Advect__

#include "../Discretization.h"
#include "../BorderCondition.h"
#include "MACGrid.h"
class Advect {
protected:
	Discretization *discretization;
	BorderCondition borderCondition;
	Discretization *normalDiscretization;
public:
    virtual ~Advect(){};
    virtual void advect(MACGrid &u, GridField<double> *g,GridField<double> *temp, double dt) = 0;
	virtual void setDiscretization(Discretization *discret, Discretization *norm)
	{ 
		discretization = discret; 
		normalDiscretization = norm;
	};
};
#endif /* defined(__FuidFire__Advect__) */
