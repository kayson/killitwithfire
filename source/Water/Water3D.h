//
//  Water3D.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/24/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FuidFire__Water3D__
#define __FuidFire__Water3D__


#include <iostream>
#include "../MACGrid.h"
#include "Particles.h"
#include "../Pressure/Projection3D.h"

class Water3D {
    
private:
    //Velocity field (MACGRID)
    MACGrid u;
    
    //Grid
    GridField<int> cellTypes;
    
    //Marker partices
    Particles particles;
    
    //Routines
    PCGProjection3D *_projection;
public:
    Vector3 g;
    double rho;
    MACAdvect<double> *_advect;
    
private:
    void recomputeCellTypes();
    
    void drawMAC();
    void drawParticles();
    void drawCenterVelocities();
    void drawCellTypes();
    void drawFaceVelocities();
    void drawVoxel(double x,double y,double z, double dx,double dy,double dz);
    void drawSphere(float r, int segs);
public:
    Water3D(int dim = 15);
    void draw();
    void runSimulation(double dt);
};

#endif /* defined(__FuidFire__Water3D__) */
