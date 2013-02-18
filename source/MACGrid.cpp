//
//  MACGrid.cpp
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/14/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#include "MACGrid.h"
#ifdef __APPLE__
#include "glfw.h"
#elif defined _WIN32 || defined _WIN64
#include <GL/glfw.h> // Takes care of everything GL-related
#include <GL/freeglut.h> // Takes care of everything GL-related
#include <math.h>
#define M_PI 3.14159265358979323846264338
#endif

/*
MACGrid::MACGrid():MACGrid(10,10,10,50){
    
}
*/

#include "GridMapping.h"

MACGrid::MACGrid(){
    _buffer = nullptr;
    initialize(10,10,10,50);
}
MACGrid::MACGrid(int dim, double size){
    initialize(dim,dim,dim,size);
}
MACGrid::MACGrid(int xdim,int ydim,int zdim, double size){
    initialize(xdim,ydim,zdim,size);
}

void MACGrid::initialize(int xdim,int ydim,int zdim, double size){
    _boxes = GridMapping(xdim,ydim,zdim, glm::mat4x4(size,0,0,0, 0,size,0,0, 0,0,size,0, 0,0,0,1) );
    
    double dx = size/(double)_boxes.xdim();
    double dy = size/(double)_boxes.ydim();
    double dz = size/(double)_boxes.zdim();
    
    _buffer = nullptr;
    
    //Transformera
    double *matrix = new double[16];
    std::fill(matrix, matrix+16, 0);
    matrix[0] = size; matrix[5] = size; matrix[10] = size; //Skala
    //matrix[3] = 0.5*_dx; matrix[7] = 0.5*_dx; matrix[11] = 0.5*_dx;
    matrix[15] = 1;
    
    
    _u = new GridField<double>(xdim+1,ydim,zdim);
    _v = new GridField<double>(xdim,ydim+1,zdim);
    _w = new GridField<double>(xdim,ydim,zdim+1);
    
    _center = new GridField<double>(xdim,ydim,zdim);
    _cache = new GridField<Vector3>(xdim,ydim,zdim);
    _cacheFlag = new GridField<bool>(xdim,ydim,zdim);
    _fluid = GridMapping(xdim*3,ydim*3,zdim*3, glm::mat4x4(size,0,0,dx*0.5, 0,size,0,dy*0.5, 0,0,size,dz*0.5, 0,0,0,1) );
    
    //Transformera
    double *u_matrix = new double[16];
    std::fill(u_matrix, u_matrix+16, 0);
    u_matrix[0] = size+dx; u_matrix[5] = size; u_matrix[10] = size; //Skala
    u_matrix[3] = 0; u_matrix[7] = dy*0.5; u_matrix[11] = dz*0.5; u_matrix[15] = 1;    //u_matrix[0] = dx*cos(0.4); u_matrix[1] = -sin(0.4)*dx; u_matrix[4] = sin(0.4)*dx; u_matrix[5] = cos(0.4)*dx; u_matrix[10] = dx; u_matrix[7] = dx*0.5; u_matrix[15] = 1;
    
    
    double *v_matrix = new double[16];
    std::fill(v_matrix, v_matrix+16, 0);
    v_matrix[0] = size; v_matrix[5] = size+dy; v_matrix[10] = size; //Skala
    v_matrix[3] = dx*0.5; v_matrix[7] = 0; v_matrix[11] = dz*0.5; v_matrix[15] = 1;
    
    double *w_matrix = new double[16];
    std::fill(w_matrix, w_matrix+16, 0);
    w_matrix[0] = size; w_matrix[5] = size; w_matrix[10] = size+dz; //Skala
    w_matrix[3] = dx*0.5; w_matrix[7] = dy*0.5; w_matrix[11] = 0; w_matrix[15] = 1;
    
    double *c_matrix = new double[16];
    std::fill(c_matrix, c_matrix+16, 0);
    c_matrix[0] = size; c_matrix[5] = size; c_matrix[10] = size; //Skala
    c_matrix[3] = dx*0.5; c_matrix[7] = dy*0.5; c_matrix[11] = dz*0.5; c_matrix[15] = 1;
    
    _u->mapping.setTransformation(u_matrix);
    _v->mapping.setTransformation(v_matrix);
    _w->mapping.setTransformation(w_matrix);
    
    _center->mapping.setTransformation(c_matrix);
    
    //Fill U
    for (GridFieldIterator<double> iterator = _u->iterator(); !iterator.done(); iterator.next()) {
        int i,j,k;
        iterator.index(i, j, k);
        double v1 = ((double)(rand() % RAND_MAX))/((double)RAND_MAX)*0.2-0.1;
        _u->setValueAtIndex(v1, iterator.index());
    }
    
    //Fill V
    for (GridFieldIterator<double> iterator = _v->iterator(); !iterator.done(); iterator.next()) {
        int i,j,k;
        iterator.index(i, j, k);
        double v1 =  ((double)(rand() % RAND_MAX))/((double)RAND_MAX)*0.2-0.1;
        _v->setValueAtIndex(v1, iterator.index());
    }
    
    //Fill W
    for (GridFieldIterator<double> iterator = _w->iterator(); !iterator.done(); iterator.next()) {
        int i,j,k;
        iterator.index(i, j, k);
        double v1  = ((double)(rand() % RAND_MAX))/((double)RAND_MAX)*0.2-0.1;
        _w->setValueAtIndex(0*v1, iterator.index());
    }
    
    //Fill Center
    for (GridFieldIterator<double> iterator = _center->iterator(); !iterator.done(); iterator.next()) {
        int i,j,k;
        iterator.index(i, j, k);
		if (sqrt((double)i*i+(double)j*j+(double)k*k) < ((double)_center->mapping.xdim())*0.5 ) {
            _center->setValueAtIndex(0.8, iterator.index());
        }else{
            _center->setValueAtIndex(0.5, iterator.index());
        }
    }
    
    
    _cacheFlag->setAll(false);
    
    //_u->setValueAtIndex(0.1, 4, 4, 4);
    //_center->setValueAtIndex(-100.0, 4, 4, 4);
}

MACGrid::MACGrid(const MACGrid &m){

    _u = new GridField<double>(*m._u);
    _v = new GridField<double>(*m._v);
    _w = new GridField<double>(*m._w);
    _center = new GridField<double>(*m._center);
    _cacheFlag = new GridField<bool>(*m._cacheFlag);
    _cache = new GridField<Vector3>(*m._cache);
    _buffer = nullptr;

}

MACGrid::~MACGrid(){
    delete _u;
    delete _v;
    delete _w;
    delete _cacheFlag;
    delete _cache;
    
    if (_buffer != nullptr) {
        delete _buffer;
    }
}
MACGrid& MACGrid::operator=(const MACGrid &g){
    
    if (this != &g) {
        
        _buffer = nullptr;
        _u = new GridField<double>(*g._u);
        _v = new GridField<double>(*g._v);
        _w = new GridField<double>(*g._w);
        _center = new GridField<double>(*g._center);
        _cache = new GridField<Vector3>(*g._cache);
        _cacheFlag = new GridField<bool>(*g._cacheFlag);
     
    }
    
    return *this;
}

MACGrid * MACGrid::buffer(){
    if (_buffer == nullptr) {
        _buffer = new MACGrid(*this);
    }
    return _buffer;
}

void MACGrid::swapBuffer(){
    GridField<double> *tmp_u,*tmp_v,*tmp_w,*tmp_c;
    tmp_u = buffer()->_u;
    tmp_v = buffer()->_v;
    tmp_w = buffer()->_w;
    tmp_c = buffer()->_center;
    
    buffer()->_u = _u;
    buffer()->_v = _v;
    buffer()->_w = _w;
    buffer()->_center = _center;
    _u = tmp_u;
    _v = tmp_v;
    _w = tmp_w;
    _center = tmp_c;
    
    if (_u == buffer()->_u) {
        assert(false);
    }
}


Vector3 MACGrid::velocityAtWorld(const Vector3 &world) const{
    return Vector3(0.1,0,0);
    double u,v,w;
    //U
    u = _u->valueAtWorld(world.x, world.y, world.z);
    //V
    v = _v->valueAtWorld(world.x, world.y, world.z);
    //W
    w = _w->valueAtWorld(world.x, world.y, world.z);
    
    return Vector3(u,v,w);
}

Vector3 MACGrid::velocityAtIndex(const Vector3 &index) const{

    double x,y,z;
    _center->mapping.indexToWorld(index.x, index.y, index.z, x, y, z);
    
    if (_cacheFlag->valueAtIndex(index.x, index.y, index.z)) {
        return _cache->valueAtIndex(index.x, index.y, index.z);
    }else{
        //return Vector3(10.0,0,0);
        
        double u,v,w;
        //U
        u = _u->valueAtWorld(x, y, z);
        //V
        v = _v->valueAtWorld(x, y, z);
        //W
        w = _w->valueAtWorld(x, y, z);
        
        Vector3 vel(u,v,w);

        _cacheFlag->setValueAtIndex(true,index.x, index.y, index.z);
        _cache->setValueAtIndex(vel,index.x, index.y, index.z);
        
        return vel;
    }
}

Vector3 MACGrid::operator()(int i ,int j,int k) const{
    return velocityAtIndex(Vector3(i,j,k));
}

void MACGrid::fillVelocity(Vector3 vel){
    
    for (GridFieldIterator<double> iter = _u->iterator(); !iter.done(); iter.next()) {
        _u->setValueAtIndex(vel.x, iter.index());
        buffer()->_u->setValueAtIndex(vel.x,iter.index());
    }
    
    for (GridFieldIterator<double> iter = _v->iterator(); !iter.done(); iter.next()) {
        _v->setValueAtIndex(vel.y, iter.index());
        buffer()->_v->setValueAtIndex(vel.y,iter.index());
    }
    
    for (GridFieldIterator<double> iter = _w->iterator(); !iter.done(); iter.next()) {
        _w->setValueAtIndex(vel.z, iter.index());
        buffer()->_w->setValueAtIndex(vel.z,iter.index());
    }
}

double MACGrid::valueAtFace(const int i,const int j,const int k, DirectionEnums d) const{
    
    if (d == RIGHT){
        return _u->valueAtIndex(i+1,j,k);
    }else if (d == LEFT){
        return _u->valueAtIndex(i,j,k);
    }else if (d == UP) {
        return _v->valueAtIndex(i,j,k);
    }else if (d == DOWN){
        return _v->valueAtIndex(i,j+1,k);
    }else  if (d == BACKWARD){
        return _w->valueAtIndex(i,j,k);
    }else{
        //Forward
        return _w->valueAtIndex(i,j,k+1);
    }
}

void MACGrid::setValueAtFace(double val,const int i, const int j, const int k, DirectionEnums d){
    if (d == RIGHT){
        _u->setValueAtIndex(val, i+1, j, k);
    }else if (d == LEFT){
        _u->setValueAtIndex(val, i, j, k);
    }else if (d == UP) {
        _v->setValueAtIndex(val, i, j, k);
    }else if (d == DOWN){
        _v->setValueAtIndex(val, i, j+1, k);
    }else  if (d == BACKWARD){
        _w->setValueAtIndex(val, i, j, k);
    }else{
        //Forward
        _w->setValueAtIndex(val, i, j+1, k);
    }
}

void drawSphere(float r, int segs) {
    int i, j;
    float x, y, z, z1, z2, R, R1, R2;
    
    // Top cap: a triangle fan
    glBegin(GL_TRIANGLE_FAN);
    //glColor4d(0.0, 0.0, 0.5, 0.9);
    //glColor3f(0.0, 0.5, 0.7);
    glNormal3f(0,0,1);
    glVertex3f(0,0,r);
    z = cos(M_PI/segs);
    R = sin(M_PI/segs);
    for(i = 0; i <= 2*segs; i++) {
        x = R*cos(i*2.0*M_PI/(2*segs));
        y = R*sin(i*2.0*M_PI/(2*segs));
        glNormal3f(x, y, z);
        glVertex3f(r*x, r*y, r*z);
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
            glNormal3f(x, y, z1);
            glVertex3f(r*x, r*y, r*z1);
            x = R2*cos(i*2.0*M_PI/(2*segs));
            y = R2*sin(i*2.0*M_PI/(2*segs));
            glNormal3f(x, y, z2);
            glVertex3f(r*x, r*y, r*z2);
        }
        glEnd();
    }
    
    // Bottom cap: a triangle fan
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,0,-1);
    glVertex3f(0,0,-r);
    z = -cos(M_PI/segs);
    R = sin(M_PI/segs);
    //glColor3f(0.0, 0.5, 0.7);
    
    for(i = 2*segs; i >= 0; i--) {
        x = R*cos(i*2.0*M_PI/(2*segs));
        y = R*sin(i*2.0*M_PI/(2*segs));
        glNormal3f(x, y, z);
        glVertex3f(r*x, r*y, r*z);
    }
    glEnd();
}


void MACGrid::draw(){
    double t  = glfwGetTime();
    //std::cout << t << std::endl;
    glRotatef(t*4, 0, 1, 0);
    glRotatef(20, 1, 0, 0);
    /*
    for (GridFieldIterator<double> iter = _u->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        _u->mapping.indexToWorld(i, j, k, x, y, z);
        double val = _u->valueAtIndex(i, j, k);
        
        glColor3f(1,0,1);
        glBegin(GL_LINES);
        glVertex3f(x, y, z);
        glVertex3f(x + val, y, z);
        glEnd();
    }
    
    for (GridFieldIterator<double> iter = _v->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        _v->mapping.indexToWorld(i, j, k, x, y, z);
        double val = _v->valueAtIndex(i, j, k);
        
        glColor3f(0,1,0);
        glBegin(GL_LINES);
        glVertex3f(x, y, z);
        glVertex3f(x, y+val, z);
        glEnd();
    }
    
    for (GridFieldIterator<double> iter = _w->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        _w->mapping.indexToWorld(i, j, k, x, y, z);
        double val = _w->valueAtIndex(i, j, k);

        glColor3f(0,1,1);
        glBegin(GL_LINES);
        glVertex3f(x, y, z);
        glVertex3f(x, y, z+val);
        glEnd();
    }*/
    
    /*GridFieldIterator<double> itera = _center->iterator();
    //IteratorRange *range = new IteratorRange(2, 2, 2, xdim()-2, ydim()-2, zdim()-2);
    //iter.setCondition(range);
    for (; !itera.done(); itera.next()) {
        int i,j,k;
        itera.index(i, j, k);
        double x,y,z;
        _center->mapping.indexToWorld(i, j, k, x, y, z);
        double val = _center->valueAtIndex(i, j, k);
        
        glPushMatrix();
        glTranslatef(x, y, z);
        if (val > 0.2) {
            glColor4f(val, 0, val, 0.1);
            drawSphere(0.04, 2);
        }
        

        glPopMatrix();
        
    }
    */
    
    GridMappingIterator itera = _fluid.iterator();
    
    //IteratorRange *range = new IteratorRange(2, 2, 2, xdim()-2, ydim()-2, zdim()-2);
    //iter.setCondition(range);
    for (; !itera.done(); itera.next()) {
        int i,j,k;
        itera.index(i, j, k);
        double x,y,z;
        _fluid.indexToWorld(i, j, k, x, y, z);
        double val = _center->valueAtWorld(x, y, z);
        
        glPushMatrix();
        glTranslatef(x, y, z);
        if (val > 0.01) {
            glColor4f(val, val, val, 0.1);
            drawSphere(0.04, 2);
        }
        glPopMatrix();
        
    }
    
    

    
    /*
    GridMappingIterator iter = _boxes.iterator();
    //IteratorRange *range = new IteratorRange(xdim()-1, ydim()-1, zdim(), xdim()-1, ydim()-1, zdim());
    //iter.setCondition(range);
    int counter = 0;
    for (; !iter.done(); iter.next()) {
        counter++;
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z,x1,y1,z1;
        _boxes.indexToWorld(i, j, k, x, y, z);
        _boxes.indexToWorld(i+1, j+1, k+1, x1, y1, z1);

        //x1 = x+0.5*_dx; y1 = y+_dx; z1 = z+_dx;
        //double dx2 = 0.5*_dx;
        //x = x-dx2; y = y -dx2; z = z -dx2;
        glColor4f(1,1,1,0.1);
        
        glPushMatrix();

        //glTranslatef(x,y,z);
        //drawSphere(0.02,2);
        glPopMatrix();
        
        glBegin(GL_LINE_STRIP);
        //TOP
        glVertex3f(x, y, z);
        glVertex3f(x, y, z1);
        glVertex3f(x1, y, z1);
        glVertex3f(x1, y, z);
        glVertex3f(x, y, z);
        glEnd();
        //BOTTOM
        glBegin(GL_LINE_STRIP);
        glVertex3f(x, y, z);
        glVertex3f(x, y1, z);
        glVertex3f(x, y1, z1);
        glVertex3f(x1, y1, z1);
        glVertex3f(x1, y1, z);
        glVertex3f(x, y1, z);
        glEnd();
        
        //SIDES
        glBegin(GL_LINE_STRIP);
        glVertex3f(x, y, z1);
        glVertex3f(x, y1, z1);
        glEnd();

        //SIDES
        glBegin(GL_LINE_STRIP);
        glVertex3f(x1, y, z1);
        glVertex3f(x1, y1, z1);
        glEnd();
        
        //SIDES
        glBegin(GL_LINE_STRIP);
        glVertex3f(x1, y, z);
        glVertex3f(x1, y1, z);
        glEnd();
    }*/
    //std::cout << counter << std::endl;
    
    
    //glTranslatef(0.5*_dx, 0.5*_dx, 0.5*_dx);
    
}


void MACGrid::advect(double dt){
    
     for (GridFieldIterator<double> iter = _u->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        _u->mapping.indexToWorld(i, j, k, x, y, z);
        
        double u = _u->valueAtWorld(x, y, z);
        double v = _v->valueAtWorld(x, y, z);
        double w = _w->valueAtWorld(x, y, z);
        
        double val = _u->valueAtWorld(x-u*dt, y-v*dt, z-w*dt);
        //v = _v->valueAtWorld(x-u*dt, y-v*dt, z-w*dt);
        //w = _w->valueAtWorld(x-u*dt, y-v*dt, z-w*dt);
        buffer()->_u->setValueAtIndex(val, i, j, k);
    }
    
    for (GridFieldIterator<double> iter = _v->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        _v->mapping.indexToWorld(i, j, k, x, y, z);
        
        double u = _u->valueAtWorld(x, y, z);
        double v = _v->valueAtWorld(x, y, z);
        double w = _w->valueAtWorld(x, y, z);
        
        double val = _v->valueAtWorld(x-u*dt, y-v*dt, z-w*dt);
        //v = _v->valueAtWorld(x-u*dt, y-v*dt, z-w*dt);
        //w = _w->valueAtWorld(x-u*dt, y-v*dt, z-w*dt);
        buffer()->_v->setValueAtIndex(val, i, j, k);
    }
    
    for (GridFieldIterator<double> iter = _w->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        _w->mapping.indexToWorld(i, j, k, x, y, z);
        
        double u = _u->valueAtWorld(x, y, z);
        double v = _v->valueAtWorld(x, y, z);
        double w = _w->valueAtWorld(x, y, z);
        
        double val = _w->valueAtWorld(x-u*dt, y-v*dt, z-w*dt);
        //v = _v->valueAtWorld(x-u*dt, y-v*dt, z-w*dt);
        //w = _w->valueAtWorld(x-u*dt, y-v*dt, z-w*dt);
        buffer()->_w->setValueAtIndex(val, i, j, k);
    }
    
    
    
    for (GridFieldIterator<double> iter = _center->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        _center->mapping.indexToWorld(i, j, k, x, y, z);
        
        double u = _u->valueAtWorld(x, y, z);
        double v = _v->valueAtWorld(x, y, z);
        double w = _w->valueAtWorld(x, y, z);
        
        double val = _center->valueAtWorld(x-u*dt, y-v*dt, z-w*dt);
        //v = _v->valueAtWorld(x-u*dt, y-v*dt, z-w*dt);
        //w = _w->valueAtWorld(x-u*dt, y-v*dt, z-w*dt);
        buffer()->_center->setValueAtIndex(val, i, j, k);
    }
    
    /*
    static Vector3 ball = Vector3(0.3,0.3,0.3);
    double u = _u->valueAtWorld(ball.x, ball.y, ball.z);
    double v = _v->valueAtWorld(ball.x, ball.y, ball.z);
    double w = _w->valueAtWorld(ball.x, ball.y, ball.z);
    ball += Vector3(u,v,w)*dt;
    glTranslatef(ball.x,ball.y,ball.z);
    drawSphere(0.05, 10);
    */
    swapBuffer();
}













