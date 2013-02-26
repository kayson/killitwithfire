//
//  Particles.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/21/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FuidFire__Particles__
#define __FuidFire__Particles__

#include "../Vector3.h"
#include "../MACGrid.h"
#include <iostream>
#include <vector>


class Particles : public std::vector<Vector3> {
public:
     
public:
    void integrateEuler(const MACGrid& grid, double dt){
        int i;
    #pragma omp parallel for private (i)
        for (i = 0; i < size(); i++) {
            
            (*this)[i] += grid.velocityAtWorld(this->operator[](i))*dt;
        }
    }
    
    void integrateRK3(const MACGrid& grid, double dt){
        int i;
    #pragma omp parallel for private (i)
        for (i = 0; i < size(); i++) {
            
            Vector3 k1 = grid.velocityAtWorld((*this)[i]);
            Vector3 k2 = grid.velocityAtWorld((*this)[i]+k1*dt*0.5);
            Vector3 k3 = grid.velocityAtWorld((*this)[i]+k2*dt*0.75);
            
            (*this)[i] += k1*2.0/9.0*dt+k2*3.0/9.0*dt+k3*4.0/9.0*dt;
        }
    }
};




#endif /* defined(__FuidFire__Particles__) */
