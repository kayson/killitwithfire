//
//  MACGrid.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/14/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FuidFire__MACGrid__
#define __FuidFire__MACGrid__

class Vector3;
template<class T> class GridField;
template<class T> class Interpolation;
#include <iostream>

#include "GridMapping.h"
#include "helper.h"


template<class T>
class MACAdvect;

class MACGrid : public GridMapping{
public:
    GridField<double> *_u,*_v,*_w;    
    GridField<bool> *_hasCache;
    GridField<Vector3> *_cache;
    MACGrid *_buffer;

    MACAdvect<double> *_advect;
protected:
    virtual void initialize(int xdim,int ydim,int zdim, double size);
public:
    
    //Factory-methods
    static MACGrid createRandom2D(int xdim,int ydim, double size);
    static MACGrid createRandom3D(int xdim,int ydim, int zdim, double size);

    //Konstruktorer
    MACGrid();
    MACGrid(int dim, double size);
    MACGrid(int xdim,int ydim,int zdim, double size);
    MACGrid(const MACGrid &m);
    ~MACGrid();
    MACGrid& operator=(const MACGrid &g);
    
    //Advektion
    //void setAdvection(MACAdvect<double> *advect){  _advect = advect;};

    //Interpolation
    template<class T>
    void setInterpolation(Interpolation<T> *interpolation);

    //Transformation
    void setTransformation(glm::dmat4x4 t);
    void multTransformation(glm::dmat4x4 t);
    
    //Buffer & cache
    MACGrid * buffer();
    void swapBuffer();
    void resetCache();

    //Index to...
    void halfIndexToWorld(int i,int j,int k, DirectionEnums d, double &x,double &y, double &z) const;
    
    //Värden
    double getMax() const;
    Vector3 velocityAtWorld(const Vector3 &world) const;
    Vector3 velocityAtCenter(int i,int j,int k) const;
    Vector3 operator()(int i ,int j,int k) const;
    
    double valueAtFace(const int i,const int j,const int k, DirectionEnums d) const;
    void setValueAtFace(double val,const int i, const int j, const int k, DirectionEnums d);
    void addValueAtFace(double val,const int i, const int j, const int k, DirectionEnums d);
    
    void addForce(Vector3 vec, double dt);
	void addForceGrid(GridField<Vector3> &f, double dt);
    void extrapolate(double dt, GridField<int > &cellType);
    void extrapolate3D(double dt, GridField<int > &cellType);

    friend class Fire;
};




#endif /* defined(__FuidFire__MACGrid__) */
