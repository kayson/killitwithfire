
#ifndef __FuidFire__Advect__
#define __FuidFire__Advect__

#include "Grid.h"
class Advect {
public:
    virtual ~Advect(){};
    virtual void advect(Grid &grid, double dt) = 0;
};
#endif /* defined(__FuidFire__Advect__) */
