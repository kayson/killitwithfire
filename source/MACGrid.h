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
#include "helper.h"

template<class T>
class MACAdvect;

class MACGrid{
private:
    MACGrid *_buffer;
    GridField<double> *_u,*_v,*_w;
    GridMapping _boxes;
    
    GridField<bool> *_cacheFlag;
    GridField<Vector3> *_cache;
    
    MACAdvect<double> *_advect;
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
    
    //Inställningar
    void setAdvection(MACAdvect<double> *advect){  _advect = advect;};

    //Buffer & cache
    MACGrid * buffer();
    void swapBuffer();
    void resetCache();

    //Värden
	unsigned int xdim(){ return _u->xdim(); }
	unsigned int ydim(){ return _u->ydim(); }
	unsigned int zdim(){ return _u->xdim(); }
	GridField<double>* getCenterField() { return _center; };
    double getMax() const;
    Vector3 velocityAtWorld(const Vector3 &world) const;
    Vector3 velocityAtCenter(int i,int j,int k) const;
    Vector3 operator()(int i ,int j,int k) const;
    void fillVelocity(Vector3 vel);
    double valueAtFace(const int i,const int j,const int k, DirectionEnums d) const;
    void setValueAtFace(double val,const int i, const int j, const int k, DirectionEnums d);
    void advect(double dt);
    void addForce(Vector3 vec, double dt);
};




#endif /* defined(__FuidFire__MACGrid__) */
