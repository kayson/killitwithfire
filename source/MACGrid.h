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
    
    GridField<bool> *_cacheFlag;
    GridField<Vector3> *_cache;
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


    //Buffer & cache
    MACGrid * buffer();
    void swapBuffer();
    void resetCache();

    //Värden
    double getMax() const;
    Vector3 velocityAtWorld(const Vector3 &world) const;
    Vector3 velocityAtIndex(const Vector3 &index) const;
    Vector3 operator()(int i ,int j,int k) const;
    void fillVelocity(Vector3 vel);
    double valueAtFace(const int i,const int j,const int k, DirectionEnums d) const;
    void setValueAtFace(double val,const int i, const int j, const int k, DirectionEnums d);
    void advect(double dt);

    void draw();
};
#endif /* defined(__FuidFire__MACGrid__) */
