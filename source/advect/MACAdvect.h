//
//  SelfAdvect.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/18/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FuidFire__SelfAdvect__
#define __FuidFire__SelfAdvect__

#include "../MACGrid.h"
#include "../GridField.h"

template<class T>
class MACAdvect {
public:
	MACAdvect(){}
    virtual ~MACAdvect(){}
    void advect(MACGrid &u, double dt){
        
        for (GridFieldIterator<double> iter = u._u->iterator(); !iter.done(); iter.next()) {
            int i,j,k;
            iter.index(i, j, k);
            double x,y,z;
            u._u->indexToWorld(i, j, k, x, y, z);
            double val = advect(dt, u, *u._u, i, j, k);
            u.buffer()->_u->setValueAtIndex(val, i, j, k);
        }
        
        for (GridFieldIterator<double> iter = u._v->iterator(); !iter.done(); iter.next()) {
            int i,j,k;
            iter.index(i, j, k);
            double x,y,z;
            u._v->indexToWorld(i, j, k, x, y, z);
            double val = advect(dt, u, *u._v, i, j, k);
            u.buffer()->_v->setValueAtIndex(val, i, j, k);
        }
        
        for (GridFieldIterator<double> iter = u._w->iterator(); !iter.done(); iter.next()) {
            int i,j,k;
            iter.index(i, j, k);
            double x,y,z;
            u._w->indexToWorld(i, j, k, x, y, z);
            double val = advect(dt, u, *u._w, i, j, k);
            u.buffer()->_w->setValueAtIndex(val, i, j, k);
        }
        
        u.swapBuffer();
    }
    void advect(MACGrid &u,GridField<double> &phi, double dt){
        for (GridFieldIterator<double> iter = u._u->iterator(); !iter.done(); iter.next()) {
            int i,j,k;
            iter.index(i, j, k);
            double x,y,z;
            u._u->indexToWorld(i, j, k, x, y, z);
            double val = advect(dt, u, *u._u,phi, i, j, k);
            u.buffer()->_u->setValueAtIndex(val, i, j, k);
        }
        
        for (GridFieldIterator<double> iter = u._v->iterator(); !iter.done(); iter.next()) {
            int i,j,k;
            iter.index(i, j, k);
            double x,y,z;
            u._v->indexToWorld(i, j, k, x, y, z);
            double val = advect(dt, u, *u._v,phi, i, j, k);
            u.buffer()->_v->setValueAtIndex(val, i, j, k);
        }
        
        for (GridFieldIterator<double> iter = u._w->iterator(); !iter.done(); iter.next()) {
            int i,j,k;
            iter.index(i, j, k);
            double x,y,z;
            u._w->indexToWorld(i, j, k, x, y, z);
            double val = advect(dt, u, *u._w,phi, i, j, k);
            u.buffer()->_w->setValueAtIndex(val, i, j, k);
        }
        
        u.swapBuffer();
    }
    
    virtual double advect(double dt,const MACGrid &g, GridField<T> &field, int i,int j,int k) = 0;
    virtual double advect(double dt,const MACGrid &g, GridField<T> &field, GridField<T> &phi, int i,int j,int k) = 0;

};

template<class T>
class MACAdvectEuler : public MACAdvect<T> {
public:
    virtual ~MACAdvectEuler(){}
    
    virtual double advect(double dt,const MACGrid &g, GridField<T> &field, int i,int j,int k){
        
        double x,y,z;
        field.indexToWorld(i,j,k,x,y,z);
        Vector3 pos = Vector3(x,y,z);
        Vector3 vel = g.velocityAtWorld(pos);
        pos = Vector3(x,y,z)-vel*dt;
        return field.valueAtWorld(pos.x,pos.y,pos.z);
    }
    
    virtual double advect(double dt,const MACGrid &g, GridField<T> &field, GridField<T> &phi, int i,int j,int k){
        return 0;
    }
};

template<class T>
class MACAdvectRK2 : public MACAdvect<T> {
public:
    virtual ~MACAdvectRK2(){};
    
    virtual double advect(double dt,const MACGrid &g, GridField<T> &field, int i,int j,int k){
        
        double x,y,z;
        field.indexToWorld(i,j,k,x,y,z);
        Vector3 pos = Vector3(x,y,z);
        Vector3 vel = g.velocityAtWorld(pos);
        pos = Vector3(x,y,z)-vel*0.5*dt;
        vel = g.velocityAtWorld(pos);
        pos = Vector3(x,y,z)-vel*dt;
        return field.valueAtWorld(pos.x,pos.y,pos.z);
    }
    
    virtual double advect(double dt,const MACGrid &g, GridField<T> &field, GridField<T> &phi, int i,int j,int k){
        return 0;

    }

};

#endif /* defined(__FuidFire__SelfAdvect__) */
