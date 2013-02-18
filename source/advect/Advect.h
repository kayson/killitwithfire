
#ifndef __FuidFire__Advect__
#define __FuidFire__Advect__

#include "../Discretization.h"
#include "../BorderCondition.h"
#include "../MACGrid.h"

class Advect {
protected:
	BorderCondition borderCondition;
	Discretization *upwindDiscretization;
	
	Discretization *centralDiscretization;
public:
    virtual ~Advect(){};

    
    virtual void advect(MACGrid &u, GridField<double> *g,GridField<double> *temp, double dt) = 0;

	virtual void setDiscretization(Discretization *upwind, Discretization *central)
	{ 
		upwindDiscretization = upwind; 
		centralDiscretization = central;
	};
};
#endif /* defined(__FuidFire__Advect__) */
