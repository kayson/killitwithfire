//
//  MACGrid.cpp
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/14/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#include "MACGrid.h"


MACGrid::MACGrid(int x,int y,int z, double dx):_u(x+1,y,z),_v(x,y+1,z),_w(x,y,z+1){
    
    //Transformera
    double *u_matrix = new double[16];
    std::fill(u_matrix, u_matrix+16, 0);
    u_matrix[0] = dx; u_matrix[5] = dx; u_matrix[10] = dx;  u_matrix[3] = 0.5*dx;
    
    double *v_matrix = new double[16];
    std::fill(v_matrix, v_matrix+16, 0);
    v_matrix[0] = dx; v_matrix[5] = dx; v_matrix[10] = dx;  v_matrix[7] = 0.5*dx;
    
    double *w_matrix = new double[16];
    std::fill(w_matrix, w_matrix+16, 0);
    w_matrix[0] = dx; w_matrix[5] = dx; w_matrix[10] = dx;  w_matrix[11] = 0.5*dx;
    
    
    //Fill U
    for (GridFieldIterator<double> iterator = _u.iterator(); !iterator.done(); iterator.next()) {
        int i,j,k;
        iterator.index(i, j, k);
        iterator.setValue(i*j*k);
    }
    
    //Fill V
    for (GridFieldIterator<double> iterator = _v.iterator(); !iterator.done(); iterator.next()) {
        int i,j,k;
        iterator.index(i, j, k);
        iterator.setValue(i*j*k);
    }
    
    //Fill W
    for (GridFieldIterator<double> iterator = _w.iterator(); !iterator.done(); iterator.next()) {
        int i,j,k;
        iterator.index(i, j, k);
        iterator.setValue(i*j*k);
    }
    
    
    
}


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
