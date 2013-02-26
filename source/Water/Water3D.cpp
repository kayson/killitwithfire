//
//  Water3D.cpp
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/24/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#include "Water3D.h"
#include "Projection3D.h"
#include "../GridFieldFileManager.h"
#include "glfw.h"

#include "../Interpolation.h"
#include "../advect/MACAdvect.h"


#ifdef __APPLE__
#include "transform.hpp"
#elif defined _WIN32 || defined _WIN64
#include <glm/gtx/transform.hpp>
#define round(x) floor((x) >= 0 ? (x) + 0.5 : (x) - 0.5)
#endif

Water3D::Water3D(int dim):u(dim,dim,dim,2600),cellTypes(dim,dim,dim,2600){
    
    //Default variables
    g = Vector3(0.0,-3.0,0.0);
    rho = 1.0;
    
    //
    _advect = new MACAdvectRK2<double>();
    
    //Translate dude
    u.multTransformation(glm::translate(-1300.0, -600.0, -800.0));
    //cellTypes.multTransformation(glm::translate(-0.5, -0.2, -0.4));
    cellTypes.setTransformation(u._trans);

    //Init environment
    //GridFieldFileManager::readFromFile<int>(cellTypes,"circle");
    
    for (GridFieldIterator<int> iter = cellTypes.iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        if (i  < 1 || (i >= cellTypes.xdim()-1) || j  < 1 || (j >= cellTypes.ydim()-1) || k  < 1 || (k >= cellTypes.zdim()-1)) {
            cellTypes.setValueAtIndex(SOLID, iter.index());
        }else{
            cellTypes.setValueAtIndex(AIR, iter.index());
        }
    }
    
    //GridFieldFileManager::writeToFile(cellTypes, "circle");
    
    
    //Init marker-particles
    for (GridFieldIterator<int> iter = cellTypes.iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double l_x,l_y,l_z;
        cellTypes.indexToLocal(i, j, k, l_x, l_y, l_z);
        double r = 0.25;
        double c_x = 0.5;
        double c_y = 0.6;
        double c_z = 0.5;

        if (r*r > (l_x-c_x)*(l_x-c_x)+(l_y-c_y)*(l_y-c_y)+(l_z-c_z)*(l_z-c_z) && iter.value() != SOLID) {
            double x,y,z;
            cellTypes.indexToWorld(i, j, k, x, y, z);
            
            particles.push_back(Vector3(x-cellTypes.dx()*0.25,y-cellTypes.dy()*0.25,z-cellTypes.dz()*0.25));
            particles.push_back(Vector3(x-cellTypes.dx()*0.25,y+cellTypes.dy()*0.25,z-cellTypes.dz()*0.25));
            particles.push_back(Vector3(x+cellTypes.dx()*0.25,y-cellTypes.dy()*0.25,z-cellTypes.dz()*0.25));
            particles.push_back(Vector3(x+cellTypes.dx()*0.25,y+cellTypes.dy()*0.25,z-cellTypes.dz()*0.25));
            
            particles.push_back(Vector3(x-cellTypes.dx()*0.25,y-cellTypes.dy()*0.25,z+cellTypes.dz()*0.25));
            particles.push_back(Vector3(x-cellTypes.dx()*0.25,y+cellTypes.dy()*0.25,z+cellTypes.dz()*0.25));
            particles.push_back(Vector3(x+cellTypes.dx()*0.25,y-cellTypes.dy()*0.25,z+cellTypes.dz()*0.25));
            particles.push_back(Vector3(x+cellTypes.dx()*0.25,y+cellTypes.dy()*0.25,z+cellTypes.dz()*0.25));
        }
        
        if (l_y < 0.25 && iter.value() != SOLID) {
            double x,y,z;
            cellTypes.indexToWorld(i, j, k, x, y, z);
            particles.push_back(Vector3(x-cellTypes.dx()*0.25,y-cellTypes.dy()*0.25,z-cellTypes.dz()*0.25));
            particles.push_back(Vector3(x-cellTypes.dx()*0.25,y+cellTypes.dy()*0.25,z-cellTypes.dz()*0.25));
            particles.push_back(Vector3(x+cellTypes.dx()*0.25,y-cellTypes.dy()*0.25,z-cellTypes.dz()*0.25));
            particles.push_back(Vector3(x+cellTypes.dx()*0.25,y+cellTypes.dy()*0.25,z-cellTypes.dz()*0.25));
            
            particles.push_back(Vector3(x-cellTypes.dx()*0.25,y-cellTypes.dy()*0.25,z+cellTypes.dz()*0.25));
            particles.push_back(Vector3(x-cellTypes.dx()*0.25,y+cellTypes.dy()*0.25,z+cellTypes.dz()*0.25));
            particles.push_back(Vector3(x+cellTypes.dx()*0.25,y-cellTypes.dy()*0.25,z+cellTypes.dz()*0.25));
            particles.push_back(Vector3(x+cellTypes.dx()*0.25,y+cellTypes.dy()*0.25,z+cellTypes.dz()*0.25));
        }
    }
    
    
    _projection = new PCGProjection3D(&u,&cellTypes);
}

void Water3D::recomputeCellTypes(){
    
    
    //Set all to AIR
    for (GridFieldIterator<int> it = cellTypes.iterator(); !it.done(); it.next()) {
        int i,j,k;
        it.index(i, j, k);
        if (cellTypes.valueAtIndex(i, j, k) != SOLID) {
            cellTypes.setValueAtIndex(AIR, it.index());
        }
    }
    
    //Set IGNITED if inhabited with marker partile
    #pragma omp parallel for
    for (int index = 0; index < particles.size(); index++) {
        int i,j,k;
        cellTypes.worldToIndex(i, j, k, particles[index].x, particles[index].y, particles[index].z);
        CellType type = static_cast<CellType>(cellTypes.valueAtIndex(i, j, k));
        if (type == AIR) {
            cellTypes.setValueAtIndex(IGNITED, i, j, k);
        }
    }
}

void Water3D::runSimulation(double dt){
    
    //Mark cells as fluid or not
    recomputeCellTypes();
    
    //Extrapolate...
    u.extrapolate3D(dt, cellTypes);
    
    //Integrera partiklar
    particles.integrateRK3(u, dt);
    
    //Advect velocity field
    MACAdvectRK2<double> advect = MACAdvectRK2<double>();
    advect.MACAdvect<double>::advect(u, dt);

    //Add exernal forces
    u.addForce(g, dt);
    
    //Project
    _projection->project(dt, rho);
    
}

void Water3D::draw(){
    
    static double deg = 30;
    deg += 0.1;
    glRotated(45, 1, 1, 0);
    glRotated(deg, 0, 1, 0);
    glScaled(1.0/1500.0, 1.0/1500.0, 1.0/1500.0);
    //drawCellTypes();
    //drawMAC();
    drawParticles();
    //drawCenterVelocities();
    //drawFaceVelocities();
}
void Water3D::drawFaceVelocities(){
    glColor3f(0,1,0);
    for (GridFieldIterator<double> iter = u._u->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        u._u->indexToWorld(i, j, k, x, y, z);
        double val = iter.value();
        glBegin(GL_LINE_STRIP);
        glVertex3d(x, y, z);
        glVertex3d(x+val, y, z);
        glEnd();
    }
    
    glColor3f(1,0,0);
    for (GridFieldIterator<double> iter = u._v->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        u._v->indexToWorld(i, j, k, x, y, z);
        double val = iter.value();
        glBegin(GL_LINE_STRIP);
        glVertex3d(x, y, z);
        glVertex3d(x, y+val, z);
        glEnd();
    }
    
    glColor3f(0,0,1);
    for (GridFieldIterator<double> iter = u._w->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        u._w->indexToWorld(i, j, k, x, y, z);
        double val = iter.value();
        glBegin(GL_LINE_STRIP);
        glVertex3d(x, y, z);
        glVertex3d(x, y, z+val);
        glEnd();
    }
    
}

void Water3D::drawCenterVelocities()
{
    
    for (GridMappingIterator iter = u.iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        u.indexToWorld(i, j, k, x, y, z);
    
    
        Vector3 v = u.velocityAtWorld(Vector3(x,y,z));//*FirePresets::dx;

        glColor3d(1.0,1.0,0.0);
        glBegin(GL_LINE_STRIP);
        glVertex3d(x, y, z);
        glVertex3d(x + v.x, y +v.y , z+v.z);
        glEnd();
    
        
    }
    
}

void Water3D::drawParticles(){
    glBegin(GL_POINTS);
    double dx = cellTypes.dx();
    for (int i = 0; i < particles.size(); i++) {
        
        glColor3d(particles[i].x/1800.0+0.5, particles[i].y/1800.0+0.5, particles[i].z/1800.0+0.5);
        //glBegin(GL_POINTS);
        glVertex3d(particles[i].x, particles[i].y, particles[i].z);
        //glEnd();
        
        
        //glVertex3d(particles[i].x, particles[i].y, particles[i].z);
        
        /*glPushMatrix();
        glTranslated(particles[i].x, particles[i].y, particles[i].z);
        drawSphere(0.25f*dx,2);
        glEnd();
        glPopMatrix();;*/
    }
    glEnd();
}
void Water3D::drawVoxel(double x,double y,double z, double dx,double dy,double dz){
    //Bottom
    glBegin(GL_LINE_STRIP);
    glVertex3d(x-dx*0.5, y-dy*0.5, z-dz*0.5);
    glVertex3d(x+dx*0.5, y-dy*0.5, z-dz*0.5);
    glVertex3d(x+dx*0.5, y+dy*0.5, z-dz*0.5);
    glVertex3d(x-dx*0.5, y+dy*0.5, z-dz*0.5);
    glVertex3d(x-dx*0.5, y-dy*0.5, z-dz*0.5);
    glEnd();
    
    //TOP
    glBegin(GL_LINE_STRIP);
    glVertex3d(x-dx*0.5, y-dy*0.5, z+dz*0.5);
    glVertex3d(x+dx*0.5, y-dy*0.5, z+dz*0.5);
    glVertex3d(x+dx*0.5, y+dy*0.5, z+dz*0.5);
    glVertex3d(x-dx*0.5, y+dy*0.5, z+dz*0.5);
    glVertex3d(x-dx*0.5, y-dy*0.5, z+dz*0.5);
    glEnd();
    
    //Connect
    glBegin(GL_LINE_STRIP);
    glVertex3d(x-dx*0.5, y-dy*0.5, z-dz*0.5);
    glVertex3d(x-dx*0.5, y-dy*0.5, z+dz*0.5);
    glEnd();
    
    glBegin(GL_LINE_STRIP);
    glVertex3d(x+dx*0.5, y-dy*0.5, z-dz*0.5);
    glVertex3d(x+dx*0.5, y-dy*0.5, z+dz*0.5);
    glEnd();
    
    glBegin(GL_LINE_STRIP);
    glVertex3d(x+dx*0.5, y+dy*0.5, z-dz*0.5);
    glVertex3d(x+dx*0.5, y+dy*0.5, z+dz*0.5);
    glEnd();
    
    glBegin(GL_LINE_STRIP);
    glVertex3d(x-dx*0.5, y+dy*0.5, z-dz*0.5);
    glVertex3d(x-dx*0.5, y+dy*0.5, z+dz*0.5);
    glEnd();
}

void Water3D::drawMAC(){
    
    
    /*
    double x,y,z;
    double x0,y0,z0,x1,y1,z1;
    cellTypes.localToWorld(0.5, 0.5, 0.5, x, y, z);
    cellTypes.localToWorld(0.0, 0.0, 0.0, x0, y0, z0);
    cellTypes.localToWorld(1.0, 1.0, 1.0, x1, y1, z1);

    double dx = x1-x0;
    double dy = y1-y0;
    double dz = z1-z0;
    
    glColor3d(1.0, 1.0, 1.0);
    drawVoxel(x, y, z, dx, dy, dz);
    
    */
    double dx = u.dx();
    double dy = u.dy();
    double dz = u.dz();
    
    for (GridMappingIterator iter = u.iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        u.indexToWorld(i, j, k, x, y, z);
        
            
        glColor3d(1.0,1.0,0.0);
        drawVoxel(x, y, z, dx, dy, dz);
        
    }
    
    
}

void Water3D::drawCellTypes(){
    
    double dx = cellTypes.dx();
    double dy = cellTypes.dy();
    double dz = cellTypes.dz();
    //double dz = cellTypes.mapping.dx();
    for (GridFieldIterator<int> iter = cellTypes.iterator(); !iter.done(); iter.next()) {
        double x,y,z;
        int i,j,k;
        iter.index(i, j, k);
        
        cellTypes.indexToWorld(i, j, k, x, y, z);
        CellType val = static_cast<CellType>(iter.value());
        
        if (val == SOLID){
            glColor3d(1.0, 1.0, 1.0);
            drawVoxel(x, y, z, dx, dy, dz);
        }/*else if (val == IGNITED){
            glColor3d(0.0, 0.0, 0.0);
            glVertex3d(x-dx*0.5, y-dy*0.5, z-dz*0.5);
            glVertex3d(x+dx*0.5, y-dy*0.5, z-dz*0.5);
            glVertex3d(x+dx*0.5, y+dy*0.5, z-dz*0.5);
            glVertex3d(x-dx*0.5, y+dy*0.5, z-dz*0.5);
        }else if (val == AIR){
            glColor3d(0.0, 0.0, 0.0);
            glVertex3d(x-dx*0.5, y-dy*0.5, z-dz*0.5);
            glVertex3d(x+dx*0.5, y-dy*0.5, z-dz*0.5);
            glVertex3d(x+dx*0.5, y+dy*0.5, z-dz*0.5);
            glVertex3d(x-dx*0.5, y+dy*0.5, z-dz*0.5);
        }*/
        //glEnd();
        
        if (val == SOLID){
            /*glColor3d(0.0, 0.0, 1.0);
            glBegin(GL_POINTS);
            glVertex3d(x, y, z);
            glVertex3d(x, y, z);
            glEnd();
            */
        }else if (val == AIR){
            glColor3d(1.0, 0.0, 0.0);
            drawVoxel(x, y, z, dx, dy, dz);
        }else if (val == IGNITED){
            glColor3d(1.0, 1.0, 0.0);
            drawVoxel(x, y, z, dx, dy, dz);
        }
        

        
    }
}

void Water3D::drawSphere(double r, int segs) {
    int i, j;
    double x, y, z, z1, z2, R, R1, R2;
    
    // Top cap: a triangle fan
    glBegin(GL_TRIANGLE_FAN);
    //glColor4d(0.0, 0.0, 0.5, 0.9);
    //glColor3f(0.0, 0.5, 0.7);
    glNormal3d(0,0,1);
    glVertex3d(0,0,r);
    z = cos(M_PI/segs);
    R = sin(M_PI/segs);
    for(i = 0; i <= 2*segs; i++) {
        x = R*cos(i*2.0*M_PI/(2*segs));
        y = R*sin(i*2.0*M_PI/(2*segs));
        glNormal3d(x, y, z);
        glVertex3d(r*x, r*y, r*z);
    }
    glEnd();
    
    // Height segments: each a triangle strip
    for(j = 1; j < segs-1; j++) {
        z1 = cos(j*M_PI/segs);
        R1 = sin(j*M_PI/segs);
        z2 = cos((j+1)*M_PI/segs);
        R2 = sin((j+1)*M_PI/segs);
        glBegin(GL_TRIANGLE_STRIP);
        //glColor3f(0.0, 0.5, 0.7);
        for(i = 0; i <= 2*segs; i++) {
            x = R1*cos(i*2.0*M_PI/(2*segs));
            y = R1*sin(i*2.0*M_PI/(2*segs));
            glNormal3d(x, y, z1);
            glVertex3d(r*x, r*y, r*z1);
            x = R2*cos(i*2.0*M_PI/(2*segs));
            y = R2*sin(i*2.0*M_PI/(2*segs));
            glNormal3d(x, y, z2);
            glVertex3d(r*x, r*y, r*z2);
        }
        glEnd();
    }
    
    // Bottom cap: a triangle fan
    glBegin(GL_TRIANGLE_FAN);
    glNormal3d(0,0,-1);
    glVertex3d(0,0,-r);
    z = -cos(M_PI/segs);
    R = sin(M_PI/segs);
    //glColor3f(0.0, 0.5, 0.7);
    
    for(i = 2*segs; i >= 0; i--) {
        x = R*cos(i*2.0*M_PI/(2*segs));
        y = R*sin(i*2.0*M_PI/(2*segs));
        glNormal3d(x, y, z);
        glVertex3d(r*x, r*y, r*z);
    }
    glEnd();
}





