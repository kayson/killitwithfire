//
//  Particles.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/21/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FuidFire__Particles__
#define __FuidFire__Particles__

#include "Vector3.h"
#include "MACGrid.h"
#include <iostream>
#include <vector>


class Particles : public std::vector<Vector3> {
public:
     
public:
    void integrate(const MACGrid& grid, double dt){
    #pragma omp parallel for
        for (int i = 0; i < size(); i++) {
            (*this)[i] += grid.velocityAtWorld((*this)[i])*dt;
        }
    }
};




#endif /* defined(__FuidFire__Particles__) */
