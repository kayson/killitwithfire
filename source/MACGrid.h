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
#include "VectorGrid.h"

class MACGrid{
private:
    MACGrid *_buffer;
    GridField<double> *_u,*_v,*_w;
    GridMapping _boxes;
    GridMapping _fluid;
public:
    GridField<double> *_center;
private:
    void initialize(int xdim,int ydim,int zdim, double size);
public:
    MACGrid();
    MACGrid(int dim, double size);
    MACGrid(int xdim,int ydim,int zdim, double size);
    MACGrid(const MACGrid &m);
    ~MACGrid();
    MACGrid& operator=(const MACGrid &g);


    //Buffer
    MACGrid * buffer();
    void swapBuffer();
    Vector3 velocityAtWorld(const Vector3 &world) const;
    double velocityAtFace(const int i,const int j,const int k, DirectionEnums d) const;
    void advect();

    void draw();
};
#endif /* defined(__FuidFire__MACGrid__) */
