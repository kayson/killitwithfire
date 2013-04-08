//
//  water2D.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/23/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FuidFire__water2D__
#define __FuidFire__water2D__

#include "GridField.h"

#include <iostream>
#include "../MACGrid.h"
#include "Particles.h"
#include "Projection2D.h"

class Water2D {
    
protected:
    //Velocity field (MACGRID)
    MACGrid u;
    
    //Grid
    GridField<int> cellTypes;
    
    //Marker partices
    Particles particles;
    
    //Routines
    PCGProjection2D *_projection;
public:
    Vector3 g;
    double rho;
    
    
protected:
    
    double random(double size) const;
    void recomputeCellTypes();
    
    void drawCellTypes();
    void drawMAC();
    void drawParticles();
    void drawCircle(float cx, float cy, float r, int num_segments);
    void drawCenterVelocities();
    void drawFaceVelocities();

public:
    Water2D(int size = 25);
    virtual void runSimulation(double dt);
    virtual void draw();
    void addBlob();
};
#endif /* defined(__FuidFire__water2D__) */
