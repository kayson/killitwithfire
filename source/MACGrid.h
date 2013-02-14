//
//  MACGrid.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/14/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FuidFire__MACGrid__
#define __FuidFire__MACGrid__

#include <iostream>
#include "Vector3.h"
#include "GridField.hpp"
class MACGrid {
private:
    GridField<double> _u,_v,_w;
public:
    MACGrid(int x,int y,int z, double dx);
    Vector3 velocityAtWorld(const Vector3 &world) const;
};
#endif /* defined(__FuidFire__MACGrid__) */
