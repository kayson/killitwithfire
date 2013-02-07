

#ifndef __FuidFire__VelocityField__
#define __FuidFire__VelocityField__

#include "Grid.h"

#ifdef __APPLE__
#include "armadillo.h"
#elif defined _WIN32 || defined _WIN64
#include "armadillo"
#endif

class VelocityField{
private:
    Grid u,v,w;
public:
    VelocityField();
    arma::vec3 getVelocityAtWorldCoordinate(arma::vec3 &pos);
};
#endif /* defined(__FuidFire__VelocityField__) */
