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
#include "datatype.h"
class MACGrid {
private:
    GridField<double> _u,_v,_w;
public:
    MACGrid();
    MACGrid(int x,int y,int z, double dx);
    MACGrid(const MACGrid &m);
    ~MACGrid(){};

    void operator=(const MACGrid &m) = delete;
    Vector3 velocityAtWorld(const Vector3 &world) const;
    double velocityAtFace(const int i,const int j,const int k, DirectionEnums d) const;

    void draw();
};
#endif /* defined(__FuidFire__MACGrid__) */
