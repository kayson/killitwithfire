

#ifndef __FuidFire__AdvectRK2__
#define __FuidFire__AdvectRK2__

#include "Advect.h"
class AdvectRK2 : public AdvectLevelSet {

public:
    AdvectRK2();
    virtual void advect(MACGrid &v,GridField<double> &g, double dt);
};

#endif /* defined(__FuidFire__AdvectRK2__) */
