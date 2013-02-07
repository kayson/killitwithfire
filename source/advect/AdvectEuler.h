

#ifndef __FuidFire__AdvectEuler__
#define __FuidFire__AdvectEuler__

#include "Advect.h"
#include "VelocityField.h"

class AdvectEuler : public Advect {
public:
    AdvectEuler(){};
    virtual void advect(VelocityField &v, double dt);
};
#endif /* defined(__FuidFire__AdvectEuler__) */
