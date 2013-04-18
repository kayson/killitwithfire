//
//  GhostGrid.cpp
//  FuidFire
//
//  Created by Johannes Deligiannis on 3/12/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#include "GridField.hpp"
#include "GhostGrid.h"
#include "LevelSet.h"
#include "fire.h"
#include "firePresets.h"



template<class T>
GhostGridField<T>::GhostGridField(int xdim,int ydim,int zdim,LevelSet *p, VelocityDirection direction):GridField<T>(xdim,ydim,zdim),_levelset(p){
    _direction = direction;
}

//Hämta värden
template<class T>
T GhostGridField<T>::valueAtWorld(double w_x, double w_y,double w_z) const{
    
    int i,j,k;
    GridField<T>::worldToUpperLeftIndex(w_x, w_y, w_z, i, j, k);
    
    CellType thisType = Fire::getCellType(_levelset->grid->valueAtWorld(w_x, w_y, w_z));
    CellType c000 = Fire::getCellType(_levelset->grid->valueAtIndex(i, j, k));
    CellType c100 = Fire::getCellType(_levelset->grid->valueAtIndex(i+1, j, k));
    CellType c110 = Fire::getCellType(_levelset->grid->valueAtIndex(i+1, j+1, k));
    CellType c010 = Fire::getCellType(_levelset->grid->valueAtIndex(i, j+1, k));

    CellType c001 = Fire::getCellType(_levelset->grid->valueAtIndex(i, j, k+1));
    CellType c101 = Fire::getCellType(_levelset->grid->valueAtIndex(i+1, j, k+1));
    CellType c111 = Fire::getCellType(_levelset->grid->valueAtIndex(i+1, j+1, k+1));
    CellType c011 = Fire::getCellType(_levelset->grid->valueAtIndex(i, j+1, k+1));
    
    double v000 = GridField<T>::valueAtIndex(i,j,k)+jump(thisType, c000, i, j, k);
    double v100 = GridField<T>::valueAtIndex(i+1,j,k)+jump(thisType, c100, i+1, j, k);
    double v110 = GridField<T>::valueAtIndex(i+1,j+1,k)+jump(thisType, c110, i+1, j+1, k);
    double v010 = GridField<T>::valueAtIndex(i,j+1,k)+jump(thisType, c010, i, j+1, k);

    double v001 = GridField<T>::valueAtIndex(i,j,k+1)+jump(thisType, c001, i, j, k+1);
    double v101 = GridField<T>::valueAtIndex(i+1,j,k+1)+jump(thisType, c101, i+1, j, k+1);
    double v111 = GridField<T>::valueAtIndex(i+1,j+1,k+1)+jump(thisType, c111, i+1, j+1, k+1);
    double v011 = GridField<T>::valueAtIndex(i,j+1,k+1)+jump(thisType, c011, i, j+1, k+1);

    double w_x0,w_y0,w_z0;
    double w_x1,w_y1,w_z1;
    double x,y,z;
    
    GridField<T>::indexToWorld(i, j, k, w_x0, w_y0, w_z0);
    GridField<T>::indexToWorld(i+1, j+1, k+1, w_x1, w_y1, w_z1);
    
    x = (w_x-w_x0)/(w_x1-w_x0);
    y = (w_y-w_y0)/(w_y1-w_y0);
    z = (w_z-w_z0)/(w_z1-w_z0);
    LinearInterpolation<T> interpolation;
 
    
    double t1 = interpolation(x,v000,v100);
    double t2 = interpolation(x,v010,v110);
    double t3 = interpolation(x,v001,v101);
    double t4 = interpolation(x,v011,v111);
    
    double t5 = interpolation(y,t1,t2);
    double t6 = interpolation(y,t3,t4);
    
    double t7 = interpolation(z,t5,t6);
    
    /*
     T t1 = (*this)(x,g.valueAtIndex(i,j,k),g.valueAtIndex(i+1,j,k));
     T t2 = (*this)(x,g.valueAtIndex(i,j+1,k),g.valueAtIndex(i+1,j+1,k));
     T t3 = (*this)(x,g.valueAtIndex(i,j,k+1),g.valueAtIndex(i+1,j,k+1));
     T t4 = (*this)(x,g.valueAtIndex(i,j+1,k+1),g.valueAtIndex(i+1,j+1,k+1));
     
     T t5 = (*this)(y,t1,t2);
     T t6 = (*this)(y,t3,t4);
     
     T t7 = (*this)(z,t5,t6);
     */

	if(t7 != 0.)
		return t7;
    
    return t7;
}
template<class T>
T GhostGridField<T>::operator()(int i,int j,int k, CellType cellType) const{
    CellType thisCell = _levelset->getCellType(i, j, k);
    return GridField<T>::operator()(i,j,k)*jump(thisCell,cellType,i,j,k);
}

//Helper
template<class T>
bool GhostGridField<T>::isSame(CellType thisDomain,CellType otherDomain) const{
    return ((thisDomain == FUEL && otherDomain == FUEL) || (thisDomain == BURNT && otherDomain == BURNT) );
}

template<class T>
double GhostGridField<T>::DVn(double x,double y,double z) const{
    Vector3 normal = _levelset->getNormal(x, y, z);
    double norm = normal.norm();
    assert(norm < 1.01);
    double val;
    if (_direction == UDIR) {
        val =  normal.x;
    }else if (_direction == VDIR){
        val =  normal.y;
    }else if (_direction == WDIR){
        val =  normal.z;
    }else{
        assert(false);
    }

	double returnval = val*DV();
    
	return returnval;
}

template<class T>
double GhostGridField<T>::DV() const{
    return (FirePresets::rhof/FirePresets::rhob-1.0)*FirePresets::S;
}

template<class T>
double GhostGridField<T>::mult(CellType thisDomain,CellType otherDomain) const{
    
    if (isSame(thisDomain, otherDomain)) {
        return 0.0;
    }else if (thisDomain == FUEL && otherDomain == BURNT){
        return 1.0;
    }else if (thisDomain == BURNT && otherDomain == FUEL){
        return -1.0;
    }else{
        assert(false);

    }
}


template<class T>
double GhostGridField<T>::jump(CellType from,CellType to, int i,int j,int k) const{
    
    double m = mult(from,to);
    if (m == 0) return 0;
    double x,y,z;
    GridField<T>::indexToWorld(i,j,k,x,y,z);
	double returnval = DVn(x,y,z)*m;
	return returnval;
}

