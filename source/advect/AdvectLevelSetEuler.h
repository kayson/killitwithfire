

#ifndef __FuidFire__AdvectLevelSetEuler__
#define __FuidFire__AdvectLevelSetEuler__

#include "AdvectLevelSet.h"

class AdvectLevelSetEuler : public AdvectLevelSet {
public:
    AdvectLevelSetEuler(){}

    virtual void advect(MACGrid &u, GridField<double> **g,GridField<double> **gridCopy, double dt);

	void integrateEuler(MACGrid &u, GridField<double> &g, double dt);
	double evaluate(MACGrid &u, GridField<double> &g, unsigned int i, unsigned int j, unsigned int k);
};
#endif /* defined(__FuidFire__AdvectLevelSetEuler__) */
