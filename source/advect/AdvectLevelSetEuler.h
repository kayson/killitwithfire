

#ifndef __FuidFire__AdvectLevelSetEuler__
#define __FuidFire__AdvectLevelSetEuler__

#include "AdvectLevelSet.h"

class AdvectLevelSetEuler : public AdvectLevelSet {
public:
    AdvectLevelSetEuler(){}

    virtual void advect(GridField<Vector3> &w, LevelSet &phi, double dt);

	void integrateEuler(MACGrid &u, GridField<double> &g, double dt);
	double evaluate(GridField<Vector3> &w, LevelSet &phi, unsigned int i, unsigned int j, unsigned int k);
};
#endif /* defined(__FuidFire__AdvectLevelSetEuler__) */
