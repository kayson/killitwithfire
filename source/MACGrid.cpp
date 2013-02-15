//
//  MACGrid.cpp
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/14/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#include "MACGrid.h"
#include "glfw.h"

MACGrid::MACGrid():MACGrid(10,10,10,50){
    
}

MACGrid::MACGrid(int x,int y,int z, double dx):_u(x+1,y,z),_v(x,y+1,z),_w(x,y,z+1){
    
    //Transformera
    double *u_matrix = new double[16];
    std::fill(u_matrix, u_matrix+16, 0);
    u_matrix[0] = dx; u_matrix[5] = dx; u_matrix[10] = dx; u_matrix[7] = dx*0.5; u_matrix[11] = -2*dx;  u_matrix[15] = 1;
    //u_matrix[0] = dx*cos(0.4); u_matrix[1] = -sin(0.4)*dx; u_matrix[4] = sin(0.4)*dx; u_matrix[5] = cos(0.4)*dx; u_matrix[10] = dx; u_matrix[7] = dx*0.5; u_matrix[15] = 1;

    double *v_matrix = new double[16];
    std::fill(v_matrix, v_matrix+16, 0);
    v_matrix[0] = dx; v_matrix[5] = dx; v_matrix[10] = dx; v_matrix[3] = dx*0.5; v_matrix[11] = -2*dx; v_matrix[15] = 1;
    
    double *w_matrix = new double[16];
    std::fill(w_matrix, w_matrix+16, 0);
    w_matrix[0] = dx; w_matrix[5] = dx; w_matrix[10] = dx; w_matrix[3] = dx*0.5; w_matrix[7] = dx*0.5; w_matrix[11] = -2*dx; w_matrix[15] = 1;

    
    _u.setTransformation(u_matrix);
    _v.setTransformation(v_matrix);
    _w.setTransformation(w_matrix);
    
    //Fill U
    for (GridFieldIterator<double> iterator = _u.iterator(); !iterator.done(); iterator.next()) {
        int i,j,k;
        iterator.index(i, j, k);
        _u.setValueAtIndex(0.2, iterator.index());
    }
    
    //Fill V
    for (GridFieldIterator<double> iterator = _v.iterator(); !iterator.done(); iterator.next()) {
        int i,j,k;
        iterator.index(i, j, k);
        _v.setValueAtIndex(0.2, iterator.index());
    }
    
    //Fill W
    for (GridFieldIterator<double> iterator = _w.iterator(); !iterator.done(); iterator.next()) {
        int i,j,k;
        iterator.index(i, j, k);
        _w.setValueAtIndex(0.2, iterator.index());
    }
    try {
        _u.valueAtIndex(100, 100, 100);
    } catch (std::runtime_error e) {
        std::cout << e.what() << std::endl;
    }
}

MACGrid::MACGrid(const MACGrid &m):_u(m._u),_v(m._v),_w(m._w){
    
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

double MACGrid::velocityAtFace(const int i,const int j,const int k, DirectionEnums d) const{
    
    if (d == RIGHT){
        return _u(i+1,j,k);
    }else if (d == LEFT){
        return _u(i,j,k);
    }else if (d == UP) {
        return _v(i,j,k);
    }else if (d == DOWN){
        return _v(i,j+1,k);
    }else  if (d == BACKWARD){
        return _w(i,j,k);
    }else{
        //Forward
        return _w(i,j,k+1);
    }
    
}

void MACGrid::draw(){
    double t  = glfwGetTime();
    //std::cout << t << std::endl;
    glRotatef(t*4, 1, 1, 0.4);
    for (GridFieldIterator<double> iter = _u.iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        _u.indexToWorld(i, j, k, x, y, z);
        double val = _u.valueAtIndex(i, j, k);
        
        glColor3f(1,0,1);
        glBegin(GL_LINES);
        glVertex3f(x, y, z);
        glVertex3f(x + val, y, z);
        glEnd();
    }
    
    for (GridFieldIterator<double> iter = _v.iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        _v.indexToWorld(i, j, k, x, y, z);
        double val = _v.valueAtIndex(i, j, k);
        
        glColor3f(0,1,0);
        glBegin(GL_LINES);
        glVertex3f(x, y, z);
        glVertex3f(x, y+val, z);
        glEnd();
    }
    
    for (GridFieldIterator<double> iter = _w.iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        _w.indexToWorld(i, j, k, x, y, z);
        double val = _w.valueAtIndex(i, j, k);

        glColor3f(0,1,1);
        glBegin(GL_LINES);
        glVertex3f(x, y, z);
        glVertex3f(x, y, z+val);
        glEnd();
    }
    
    
}










