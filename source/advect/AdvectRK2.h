//
//  AdvectRK2.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/7/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FuidFire__AdvectRK2__
#define __FuidFire__AdvectRK2__

#include "Advect.h"
class AdvectRK2 : public Advect {

public:
    AdvectRK2();
    virtual void advect(Grid &grid, double dt);
};

#endif /* defined(__FuidFire__AdvectRK2__) */
