
#ifndef __FuidFire__Advect__
#define __FuidFire__Advect__

#include "../VelocityField.h"
class Advect {
public:
    virtual ~Advect(){};
    virtual void advect(VelocityField &v,Grid &g, double dt) = 0;
};
#endif /* defined(__FuidFire__Advect__) */
