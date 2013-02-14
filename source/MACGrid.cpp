//
//  MACGrid.cpp
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/14/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#include "MACGrid.h"


Vector3 MACGrid::velocityAtWorld(const Vector3 &world) const{
    double u,v,w;
    //U
    u = _u.valueAtWorld(world.x, world.y, world.z);
    //V
    v = _v.valueAtWorld(world.x, world.y, world.z);
    //W
    w = _w.valueAtWorld(world.x, world.y, world.z);

    return Vector3(u,v,w);
}
