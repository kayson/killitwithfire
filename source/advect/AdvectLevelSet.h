
#ifndef __FuidFire__Advect__
#define __FuidFire__Advect__

#include "../Discretization.h"
#include "../BorderCondition.h"
#include "../levelset/LevelSet.h"
#include "../MACGrid.h"

class AdvectLevelSet {
protected:
	BorderCondition borderCondition;

public:
    virtual ~AdvectLevelSet(){}
    
    virtual void advect(GridField<Vector3> &w, LevelSet &phi, double dt) = 0;

};
#endif /* defined(__FuidFire__Advect__) */
