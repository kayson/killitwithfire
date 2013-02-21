//
//  SelfAdvect.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/18/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FuidFire__SelfAdvect__
#define __FuidFire__SelfAdvect__

#include "MACGrid.h"
#include "GridField.h"

template<class T>
class MACAdvect {
public:
    virtual ~MACAdvect(){};
    virtual double advect(double dt,const MACGrid &g, GridField<T> &field, int i,int j,int k) = 0;
};

template<class T>
class MACAdvectEuler : public MACAdvect<T> {
public:
    virtual ~MACAdvectEuler(){};
    
    virtual double advect(double dt,const MACGrid &g, GridField<T> &field, int i,int j,int k) final{
        
        double x,y,z;
        field.indexToWorld(i,j,k,x,y,z);
        Vector3 pos = Vector3(x,y,z);
        Vector3 vel = g.velocityAtWorld(pos);
        pos = Vector3(x,y,z)-vel*dt;
        return field.valueAtWorld(pos.x,pos.y,pos.z);
    }
};

template<class T>
class MACAdvectRK2 : public MACAdvect<T> {
public:
    virtual ~MACAdvectRK2(){};
    
    virtual double advect(double dt,const MACGrid &g, GridField<T> &field, int i,int j,int k) final{
        
        double x,y,z;
        field.mapping.indexToWorld(i,j,k,x,y,z);
        Vector3 pos = Vector3(x,y,z);
        Vector3 vel = g.velocityAtWorld(pos);
        pos = Vector3(x,y,z)-vel*0.5*dt;
        vel = g.velocityAtWorld(pos);
        pos = Vector3(x,y,z)-vel*dt;
        return field.valueAtWorld(pos.x,pos.y,pos.z);
    }
};

#endif /* defined(__FuidFire__SelfAdvect__) */
