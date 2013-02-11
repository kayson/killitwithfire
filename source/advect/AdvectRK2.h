

#ifndef __FuidFire__AdvectRK2__
#define __FuidFire__AdvectRK2__

#include "Advect.h"
class AdvectRK2 : public Advect {

public:
    AdvectRK2();
    virtual void advect(VelocityField &v,Grid &g, double dt);
};

#endif /* defined(__FuidFire__AdvectRK2__) */
