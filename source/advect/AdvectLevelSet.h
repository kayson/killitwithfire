
#ifndef __FuidFire__Advect__
#define __FuidFire__Advect__

#include "../BorderCondition.h"

class LevelSet;
class BorderCondition;
template<class T> class GridField;

class AdvectLevelSet {
protected:
	BorderCondition borderCondition;

public:
    virtual ~AdvectLevelSet(){}
    
    virtual void advect(GridField<Vector3> &w, LevelSet &phi, double dt) = 0;

};
#endif /* defined(__FuidFire__Advect__) */
