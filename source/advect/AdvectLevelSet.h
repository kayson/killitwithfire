
#ifndef __FuidFire__Advect__
#define __FuidFire__Advect__

#include "../Discretization.h"
#include "../BorderCondition.h"
#include "../MACGrid.h"

class AdvectLevelSet {
protected:
	BorderCondition borderCondition;

public:
    virtual ~AdvectLevelSet(){}

    
    virtual void advect(MACGrid &u, GridField<double> **g,GridField<double> **temp, double dt) = 0;

};
#endif /* defined(__FuidFire__Advect__) */
