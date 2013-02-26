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
#include "../levelset/LevelSet.h"
#include "../helper.h"
#include "../fire.h"

template<class T>
class MACAdvect {
public:
	MACAdvect(){}
    virtual ~MACAdvect(){}
    void advect(MACGrid &u, double dt){
        double x,y,z;

#pragma omp parallel for private (x,y,z)
        for (int i = 0; i < u._u->xdim(); i++) {
            for (int j = 0; j < u._u->ydim(); j++) {
                for (int k = 0; k < u._u->zdim(); k++) {
                    u._u->indexToWorld(i, j, k, x, y, z);
                    double val = advect(dt, u, *u._u, i, j, k);
                    u.buffer()->_u->setValueAtIndex(val, i, j, k);
                }
            }
        }
        
        
    #pragma omp parallel for private (x,y,z)

        for (int i = 0; i < u._v->xdim(); i++) {
            for (int j = 0; j < u._v->ydim(); j++) {
                for (int k = 0; k < u._v->zdim(); k++) {
                    u._v->indexToWorld(i, j, k, x, y, z);
                    double val = advect(dt, u, *u._v, i, j, k);
                    u.buffer()->_v->setValueAtIndex(val, i, j, k);
                }
            }
        }
        
#pragma omp parallel for private (x,y,z)
        for (int i = 0; i < u._w->xdim(); i++) {
            for (int j = 0; j < u._w->ydim(); j++) {
                for (int k = 0; k < u._w->zdim(); k++) {
                    u._w->indexToWorld(i, j, k, x, y, z);
                    double val = advect(dt, u, *u._w, i, j, k);
                    u.buffer()->_w->setValueAtIndex(val, i, j, k);
                }
            }
        }
        
        u.swapBuffer();
    }
    
    void advect(MACGrid &u, LevelSet& phi, double dt){
        for (GridFieldIterator<double> iter = u._u->iterator(); !iter.done(); iter.next()) {
            int i,j,k;
            iter.index(i, j, k);
            double x,y,z;
            u._u->indexToWorld(i, j, k, x, y, z);
            double val = advect(dt, u, VelocityDirection::UDIR , phi, i, j, k);
            u.buffer()->_u->setValueAtIndex(val, i, j, k);
        }
        
        for (GridFieldIterator<double> iter = u._v->iterator(); !iter.done(); iter.next()) {
            int i,j,k;
            iter.index(i, j, k);
            double x,y,z;
            u._v->indexToWorld(i, j, k, x, y, z);
            double val = advect(dt, u, VelocityDirection::VDIR ,phi, i, j, k);
            u.buffer()->_v->setValueAtIndex(val, i, j, k);
        }
        
        for (GridFieldIterator<double> iter = u._w->iterator(); !iter.done(); iter.next()) {
            int i,j,k;
            iter.index(i, j, k);
            double x,y,z;
            u._w->indexToWorld(i, j, k, x, y, z);
            double val = advect(dt, u, VelocityDirection::WDIR, phi, i, j, k);
            u.buffer()->_w->setValueAtIndex(val, i, j, k);
        }
        
        u.swapBuffer();
    }
    
    virtual double advect(double dt,const MACGrid &g, GridField<T> &field, int i,int j,int k) = 0;
    virtual double advect(double dt,const MACGrid &g, const VelocityDirection dir, LevelSet& phi, int i,int j,int k) = 0;

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
    
	// Vs = Velocity start, Ve = Velocity end osv
    virtual double advect(double dt,const MACGrid &g, const VelocityDirection dir, LevelSet& phi, int i, int j, int k){
		GridField<double> *field;
		
		if(dir == VelocityDirection::UDIR)
			field = g._u;
		else if(dir == VelocityDirection::VDIR)
			field = g._v;
		else //(dir == VelocityDirection::WDIR)
			field = g._w;

		double x,y,z;
        field->indexToWorld(i,j,k,x,y,z);
        
		Vector3 sPos = Vector3(x,y,z);
		CellType sType = Fire::getCellType(phi.grid->valueAtWorld(sPos.x, sPos.y, sPos.z));
        Vector3 sVel = g.velocityAtWorld(sPos);

        Vector3 ePos = sPos-sVel*dt;
		CellType eType = Fire::getCellType(phi.grid->valueAtWorld(ePos.x, ePos.y, ePos.z));

		if(sType == eType)
			return field->valueAtWorld(ePos.x,ePos.y,ePos.z);
		else
		{
			Vector3 n = phi.getNormal(sPos.x, sPos.y, sPos.z);
			double Vs = Vector3::dot(sVel, n);
			
			double Ve = 0;
			if(sType == BLUECORE)
				Ve = Vs + (FirePresets::rhof/FirePresets::rhoh - 1.0)*FirePresets::S;
			else if(sType == IGNITED)
				Ve = Vs + (FirePresets::rhoh/FirePresets::rhof - 1.0)*FirePresets::S;
			else
			{
				std::cout << "Fel i MACAdvect" << std::endl;
				throw;
			}

			Vector3 ue = Ve*n + sVel - Vs*n;
			
			if(dir == VelocityDirection::UDIR)
				return ue.x;
			else if(dir == VelocityDirection::VDIR)
				return ue.y;
			else //(dir == VelocityDirection::WDIR)
				return ue.x;
		}
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
    
    virtual double advect(double dt,const MACGrid &g, const VelocityDirection dir, LevelSet& phi, int i,int j,int k){
       /* GridField<T> *field;
		if(dir == VelocityDirection::UDIR)
			field = g._u;
		else if(dir == VelocityDirection::VDIR)
			field = g._v;
		else //(dir == VelocityDirection::WDIR)
			field = g._w;
		
		double x,y,z;
        field->indexToWorld(i,j,k,x,y,z);
        Vector3 pos = Vector3(x,y,z);
        Vector3 vel = g.velocityAtWorld(pos);
        pos = Vector3(x,y,z)-vel*0.5*dt;
        vel = g.velocityAtWorld(pos);
        pos = Vector3(x,y,z)-vel*dt;*/
        return 0.0;
    }

};

#endif /* defined(__FuidFire__SelfAdvect__) */
