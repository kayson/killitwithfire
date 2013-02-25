//
//  water2D.cpp
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/23/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#include "Water2D.h"
#include "Projection2D.h"
#include "../GridFieldFileManager.h"
#include "glfw.h"
#include "../Advect/MACAdvect.h"

Water2D::Water2D():u(30,30,1,18),cellTypes(30,30,1,18){
    
    //Default variables
    g = Vector3(0.0,-0.15,0.0);
    rho = 0.1;

    //Init environment
    //GridFieldFileManager::readFromFile<int>(cellTypes,"circle");
    
    for (GridFieldIterator<int> iter = cellTypes.iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        if (i  == 0 || i == cellTypes.xdim()-1 || j  == 0 || j == cellTypes.ydim()-1) {
            cellTypes.setValueAtIndex(SOLID, iter.index());
        }else{
            cellTypes.setValueAtIndex(AIR, iter.index());
        }
    }
    
    for (GridFieldIterator<int> iter = cellTypes.iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double l_x,l_y,l_z;
        cellTypes.indexToLocal(i, j, k, l_x, l_y, l_z);
        if (l_x > 0.4 && l_x < 0.7 && l_y > 0.2 && l_y < 0.3 ) {
            cellTypes.setValueAtIndex(SOLID, iter.index());
        }
    }
    
    //GridFieldFileManager::writeToFile(cellTypes, "circle");

    
    addBlob();
    
    _projection = new PCGProjection2D(&u,&cellTypes);
}

void Water2D::addBlob(){
    //Init marker-particles
    for (GridFieldIterator<int> iter = cellTypes.iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double l_x,l_y,l_z;
        cellTypes.indexToLocal(i, j, k, l_x, l_y, l_z);
        double r = 0.25;
        double c_x = 0.5;
        double c_y = 0.85;
        
        if (r*r > (l_x-c_x)*(l_x-c_x)+(l_y-c_y)*(l_y-c_y)+l_z*l_z && iter.value() != SOLID) {
            double x,y,z;
            cellTypes.indexToWorld(i, j, k, x, y, z);
            particles.push_back(Vector3(x-cellTypes.dx()*0.25,y-cellTypes.dy()*0.25,z));
            particles.push_back(Vector3(x-cellTypes.dx()*0.25,y+cellTypes.dy()*0.25,z));
            particles.push_back(Vector3(x+cellTypes.dx()*0.25,y-cellTypes.dy()*0.25,z));
            particles.push_back(Vector3(x+cellTypes.dx()*0.25,y+cellTypes.dy()*0.25,z));
        }
    }
}

void Water2D::recomputeCellTypes(){
    
    //Set all to AIR
    for (GridFieldIterator<int> it = cellTypes.iterator(); !it.done(); it.next()) {
        int i,j,k;
        it.index(i, j, k);
        if (cellTypes.valueAtIndex(i, j, k) != SOLID) {
            cellTypes.setValueAtIndex(AIR, it.index());
        }
    }
    
    //Set IGNITED if inhabited with marker partile
    for (int index = 0; index < particles.size(); index++) {
        int i,j,k;
        cellTypes.worldToIndex(i, j, k, particles[index].x, particles[index].y, 0);
        CellType type = static_cast<CellType>(cellTypes.valueAtIndex(i, j, k));
        if (type == AIR) {
            cellTypes.setValueAtIndex(IGNITED, i, j, k);
        }
    }
}

void Water2D::runSimulation(double dt){
    
    //Mark cells as fluid or not
    recomputeCellTypes();
    
    static int counter = 0;
    if (counter % 200 == 0) {
        addBlob();
    }
    counter++;
    
    
    //Extrapolate...
    u.extrapolate(dt, cellTypes);
    
    //Integrera partiklar
    particles.integrate(u, dt);
    
    //Advect velocity field
    MACAdvectRK2<double> advect = MACAdvectRK2<double>();
    advect.MACAdvect<double>::advect(u, dt);

    //Add exernal forces
    u.addForce(g, dt);
    
    //Project
    _projection->project(dt, rho);
    
}

void Water2D::draw(){
    
    drawCellTypes();
    //drawMAC();
    drawParticles();
    drawCenterVelocities();
    //drawFaceVelocities();

}

void Water2D::drawParticles(){
	glEnable(GL_POINT_SMOOTH);
	glPointSize(3.0f);    
    for (int i = 0; i < particles.size(); i++) {
        glColor3d(0.6, 0.6, 1.0);
        glBegin(GL_POINTS);
        glVertex3d(particles[i].x, particles[i].y, 0);
        glEnd();
    }
    
    /*
    for (int i = 0; i < particles.size(); i++) {
        glColor3d(1.0, 0.0, 0.0);
        drawCircle(particles[i].x, particles[i].y, cellTypes.dx()*0.2, 10);
    }*/
}

void Water2D::drawCircle(float cx, float cy, float r, int num_segments){
	glBegin(GL_LINE_LOOP);
	for(int ii = 0; ii < num_segments; ii++)
	{
		float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
        
		float x = r * cosf(theta);//calculate the x component
		float y = r * sinf(theta);//calculate the y component
        
		glVertex2f(x + cx, y + cy);//output vertex
        
	} 
	glEnd(); 
}


void Water2D::drawMAC(){
    double dx = u.dx();
    double dy = u.dy();
    for (GridMappingIterator iter = u.iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        u.indexToWorld(i, j, k, x, y, z);
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_STRIP);
        glVertex3d(x-dx*0.5, y-dy*0.5, 0);
        glVertex3d(x+dx*0.5, y-dy*0.5, 0);
        glVertex3d(x+dx*0.5, y+dy*0.5, 0);
        glVertex3d(x-dx*0.5, y+dy*0.5, 0);
        glVertex3d(x-dx*0.5, y-dy*0.5, 0);
        glEnd();
        
    }
}

void Water2D::drawCenterVelocities(){
    for (GridMappingIterator iter = u.iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        u.indexToWorld(i, j, k, x, y, z);
    
        Vector3 v = u.velocityAtWorld(Vector3(x,y,z));//*FirePresets::dx;
        glColor3d(1.0,1.0,0.0);
        glBegin(GL_LINE_STRIP);
        glVertex3d(x, y, 0);
        glVertex3d(x + v.x, y +v.y , 0);
        glEnd();
    }
}
void Water2D::drawFaceVelocities(){
    glColor3f(0,1,0);
    for (GridFieldIterator<double> iter = u._u->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        u._u->indexToWorld(i, j, k, x, y, z);
        double val = iter.value();
        
        glBegin(GL_LINE_STRIP);
        glVertex3d(x, y, 0);
        glVertex3d(x+val, y, 0);
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
        glVertex3d(x, y, 0);
        glVertex3d(x, y+val, 0);
        glEnd();
    }
}

void Water2D::drawCellTypes(){
    
    double dx = cellTypes.dx();
    double dy = cellTypes.dy();
    //double dz = cellTypes.mapping.dx();
    for (GridFieldIterator<int> iter = cellTypes.iterator(); !iter.done(); iter.next()) {
        double x,y,z;
        int i,j,k;
        iter.index(i, j, k);
        
        cellTypes.indexToWorld(i, j, k, x, y, z);
        int val = iter.value();
        glBegin(GL_QUADS);
        
        if (val == SOLID){
            glColor3d(1.0, 1.0, 1.0);
            glVertex3d(x-dx*0.5, y-dy*0.5, 0);
            glVertex3d(x+dx*0.5, y-dy*0.5, 0);
            glVertex3d(x+dx*0.5, y+dy*0.5, 0);
            glVertex3d(x-dx*0.5, y+dy*0.5, 0);
        }else if (val == IGNITED){
            glColor3d(0.0, 0.0, 0.0);
            glVertex3d(x-dx*0.5, y-dy*0.5, 0);
            glVertex3d(x+dx*0.5, y-dy*0.5, 0);
            glVertex3d(x+dx*0.5, y+dy*0.5, 0);
            glVertex3d(x-dx*0.5, y+dy*0.5, 0);
        }else if (val == AIR){
            glColor3d(0.0, 0.0, 0.0);
            glVertex3d(x-dx*0.5, y-dy*0.5, 0);
            glVertex3d(x+dx*0.5, y-dy*0.5, 0);
            glVertex3d(x+dx*0.5, y+dy*0.5, 0);
            glVertex3d(x-dx*0.5, y+dy*0.5, 0);
        }
        glEnd();
    }
}

