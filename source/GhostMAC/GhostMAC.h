//
//  GhostMAC.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 3/5/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FuidFire__GhostMAC__
#define __FuidFire__GhostMAC__

#include <iostream>
#include "MACGrid.h"
#include "LevelSet.h"
class GhostMAC : public MACGrid {
private:
    LevelSet *_levelset;
public:
    void makeRandom();
    GhostMAC(LevelSet *l, double size, bool allocBuffer):MACGrid(l->grid->xdim(),l->grid->ydim(),l->grid->zdim(),size),_levelset(l){
        initialize(l->grid->xdim(), l->grid->ydim(), l->grid->zdim(), size);
        if(allocBuffer) _buffer = new GhostMAC(_levelset,size, false);
    }
    double valueAtFace(int x,int y,int z,DirectionEnums dir, CellType cell) const;
    virtual void initialize(int xdim,int ydim,int zdim, double size);
};
#endif /* defined(__FuidFire__GhostMAC__) */
