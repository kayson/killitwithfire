//
//  GhostGrid.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 3/12/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FuidFire__GhostGrid__
#define __FuidFire__GhostGrid__
#include "helper.h"
class LevelSet;
template<class T> class GridField;
template<class T>
class GhostGridField : public GridField<T> {
private:
    LevelSet *_levelset;
    VelocityDirection _direction;
public:
    GhostGridField(int xdim,int ydim,int zdim,LevelSet *p, VelocityDirection direction);
    
    //Hämta värden
    virtual T valueAtWorld(double w_x, double w_y,double w_z) const;

private:
    //Helper
    double jump(CellType thisDomain,CellType otherDomain, int i,int j,int k) const;
    double DVn(double w_x,double w_y , double w_z) const;
    double DV() const;
    double mult(CellType thisDomain,CellType otherDomain) const;
    
    friend class GhostMAC;
};

#endif /* defined(__FuidFire__GhostGrid__) */
