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
#include "MACAdvect.h"
#include "transform.hpp"
#elif defined _WIN32 || defined _WIN64
#include <GL/glfw.h> // Takes care of everything GL-related
#include <GL/freeglut.h> // Takes care of everything GL-related
#include <math.h>
#include "advect\MACAdvect.h"
#include <glm/gtx/transform.hpp> 
#define M_PI 3.14159265358979323846264338
#endif
#include <algorithm>

/*
MACGrid::MACGrid():MACGrid(10,10,10,50){
    
}
*/

#include "GridMapping.h"
//Factory
MACGrid MACGrid::createRandom2D(int xdim,int ydim, double size){
    
    MACGrid m = MACGrid(xdim,ydim,1,size);
    double randMax = -0.0;
    //Fill U
    for (GridFieldIterator<double> iterator = m._u->iterator(); !iterator.done(); iterator.next()) {
        int i,j,k;
        double x,y,z;
        iterator.index(i, j, k);
        m._u->indexToWorld(i, j, k, x, y, z);
        double v1 = ((double)(rand() % RAND_MAX))/((double)RAND_MAX)*randMax;//-randMax*0.5;
        double vel = v1;
        m._u->setValueAtIndex(vel, iterator.index());
        m.buffer()->_u->setValueAtIndex(vel, iterator.index());
    }
    
    //Fill V
    for (GridFieldIterator<double> iterator = m._v->iterator(); !iterator.done(); iterator.next()) {
        int i,j,k;
        //double x,y,z;
        iterator.index(i, j, k);
        //_v->indexToWorld(i, j, k, x, y, z);
        double v1 =  ((double)(rand() % RAND_MAX))/((double)RAND_MAX)*randMax;//-randMax*0.5;
        double vel = v1;
        m._v->setValueAtIndex(vel, iterator.index());
        m.buffer()->_v->setValueAtIndex(vel, iterator.index());
        
    }
    
    //Fill W
    for (GridFieldIterator<double> iterator = m._w->iterator(); !iterator.done(); iterator.next()) {
        int i,j,k;
        //double x,y,z;
        iterator.index(i, j, k);
        double v1  = ((double)(rand() % RAND_MAX))/((double)RAND_MAX)*randMax;//-randMax*0.5;
        double vel = v1;
        m._w->setValueAtIndex(vel, iterator.index());
        m.buffer()->_w->setValueAtIndex(vel, iterator.index());
    }
    
    return m;
}

//Konstruktor
MACGrid::MACGrid():GridMapping(10,10,10,1){
    _buffer = NULL;
    initialize(10,10,10,1);
}
MACGrid::MACGrid(int dim, double size):GridMapping(dim,dim,dim,size){
    initialize(dim,dim,dim,size);
}
MACGrid::MACGrid(int xdim,int ydim,int zdim, double size):GridMapping(xdim,ydim,zdim,size){
    initialize(xdim,ydim,zdim,size);
}

void MACGrid::initialize(int xdim,int ydim,int zdim, double size){

    //Default advektionsalgoritm
    _advect = new MACAdvectRK2<double>();

    //Initiera variabler
    _buffer = NULL;
    
    _u = new GridField<double>(xdim+1,ydim,zdim);
    _v = new GridField<double>(xdim,ydim+1,zdim);
    _w = new GridField<double>(xdim,ydim,zdim+1);
    
    _cache = new GridField<Vector3>(xdim,ydim,zdim );
    _cache->setTransformation(glm::dmat4x4(size,0,0,0, 0,size,0,0, 0,0,size,0, 0,0,0,1));
    _hasCache = new GridField<bool>(xdim,ydim,zdim);
    _hasCache->setTransformation(glm::dmat4x4(size,0,0,0, 0,size,0,0, 0,0,size,0, 0,0,0,1));
    _hasCache->setAll(false);

    //Skala upp
    setTransformation(glm::scale(size, size, size));

    
    //_u->setValueAtIndex(0.1, 4, 4, 4);
    //_center->setValueAtIndex(-100.0, 4, 4, 4);
}

MACGrid::MACGrid(const MACGrid &m):GridMapping(m){
    
    _u = new GridField<double>(*m._u);
    _v = new GridField<double>(*m._v);
    _w = new GridField<double>(*m._w);
    _hasCache = new GridField<bool>(*m._hasCache);
    _cache = new GridField<Vector3>(*m._cache);
    _buffer = NULL;
     
}

MACGrid::~MACGrid(){
    delete _u;
    delete _v;
    delete _w;
    delete _hasCache;
    delete _cache;
    
    if (_buffer != NULL) {
        delete _buffer;
    }
}

MACGrid& MACGrid::operator=(const MACGrid &g){
    
    if (this != &g) {
        GridMapping::operator=(g);
        
        _buffer = NULL;
        _u = new GridField<double>(*g._u);
        _v = new GridField<double>(*g._v);
        _w = new GridField<double>(*g._w);
        _cache = new GridField<Vector3>(*g._cache);
        _hasCache = new GridField<bool>(*g._hasCache);
     
    }
    
    return *this;
}

template<class T>
void MACGrid::setInterpolation(Interpolation<T> *interpolation){
    _u->setInterpolation(interpolation);
    _v->setInterpolation(interpolation);
    _w->setInterpolation(interpolation);
}

void MACGrid::setTransformation(glm::dmat4x4 t){
    GridMapping::setTransformation(t);
    
    //Reset to original position
    //2D!
    /*
    _u->setTransformation(glm::scale(1.0+local_dx(), 1.0, 1.0));
    _u->multTransformation(glm::translate(-0.5*GridMapping::dx(), 0.0, 0.0));
    
    _v->setTransformation(glm::scale(1.0, 1.0+local_dy(), 1.0));
    _v->multTransformation(glm::translate(0.0, -0.5*GridMapping::dy() ,0.0));

    //_w->setTransformation(glm::translate(0.0, 0.0, 0.0));
    */
    
     //3D!
    _u->setTransformation(glm::translate(-0.5*GridMapping::dx(),0.0,0.0));
    _u->multTransformation(glm::scale(1.0+local_dx(), 1.0, 1.0));
    
    _v->setTransformation(glm::translate(0.0, -0.5*GridMapping::dy() ,0.0));
    _v->multTransformation(glm::scale(1.0, 1.0+local_dy(), 1.0));
    
    _w->setTransformation(glm::translate(0.0, 0.0 ,-0.5*GridMapping::dz()));
    _w->multTransformation(glm::scale(1.0, 1.0, 1.0+local_dz()));
    
    //Apply to children
    _u->multTransformation(t);
    _v->multTransformation(t);
    _w->multTransformation(t);
    
}
void MACGrid::multTransformation(glm::dmat4x4 t){
    GridMapping::setTransformation(t*_trans);
    _u->setTransformation(t*_u->_trans);
    _v->setTransformation(t*_v->_trans);
    _w->setTransformation(t*_w->_trans);
}

MACGrid * MACGrid::buffer(){
    if (_buffer == NULL) {
        _buffer = new MACGrid(*this);
    }
    return _buffer;
}

void MACGrid::swapBuffer(){

    GridField<double> *_u_buffer = &(*buffer()->_u);
    GridField<double> *_v_buffer = &(*buffer()->_v);
    GridField<double> *_w_buffer = &(*buffer()->_w);
    GridField<bool> *_hasCache_buffer = &(*buffer()->_hasCache);
    GridField<Vector3> *_cache_buffer = &(*buffer()->_cache);

    buffer()->_u = &(*_u);
    buffer()->_v = &(*_v);
    buffer()->_w = &(*_w);
    buffer()->_hasCache = &(*_hasCache);
    buffer()->_cache = &(*_cache);

    _u = &(*_u_buffer);
    _v = &(*_v_buffer);
    _w = &(*_w_buffer);
    _hasCache = &(*_hasCache_buffer);
    _cache = &(*_cache_buffer);
    
    
    /*std::iter_swap(buffer()->_u, _u);
    std::iter_swap(buffer()->_v, _v);
    std::iter_swap(buffer()->_w, _w);
     */
}

double MACGrid::getMax() const{
    return 0.1;
}

Vector3 MACGrid::velocityAtWorld(const Vector3 &world) const{

    double u,v,w;
    //U
    u = _u->valueAtWorld(world.x, world.y, world.z);
    //V
    v = _v->valueAtWorld(world.x, world.y, world.z);
    //W
    w = _w->valueAtWorld(world.x, world.y, world.z);
    
    return Vector3(u,v,w);
}

Vector3 MACGrid::velocityAtCenter(int i,int j,int k) const{

    double x,y,z;

    if (_hasCache->valueAtIndex(i, j, k)) {
        return _cache->valueAtIndex(i, j, k);
    }else{

        indexToWorld(i, j, k, x, y, z);

        double u,v,w;
        //U
        u = _u->valueAtWorld(x, y, z);
        //V
        v = _v->valueAtWorld(x, y, z);
        //W
        w = _w->valueAtWorld(x, y, z);

        Vector3 vel(u,v,w);

        //_hasCache->setValueAtIndex(true,i, j, k);
        //_cache->setValueAtIndex(vel,i, j, k);
        
        return vel;
    }
}

Vector3 MACGrid::operator()(int i ,int j,int k) const{
    return velocityAtCenter(i,j,k);
}


double MACGrid::valueAtFace(const int i,const int j,const int k, DirectionEnums d) const{
    
    if (d == RIGHT){
        return _u->valueAtIndex(i+1, j, k);
    }else if (d == LEFT){
        return _u->valueAtIndex(i, j, k);
    }else if (d == DOWN) {
        return _v->valueAtIndex(i, j, k);
    }else if (d == UP){
        return _v->valueAtIndex(i, j+1, k);
    }else  if (d == FORWARD){
        return _w->valueAtIndex(i, j, k);
    }else if(d == BACKWARD){
        return _w->valueAtIndex(i, j, k+1);
    }else{
        //Center?
    }
}

void MACGrid::setValueAtFace(double val,const int i, const int j, const int k, DirectionEnums d){
    if (d == RIGHT){
        _u->setValueAtIndex(val, i+1, j, k);
    }else if (d == LEFT){
        _u->setValueAtIndex(val, i, j, k);
    }else if (d == DOWN) {
        _v->setValueAtIndex(val, i, j, k);
    }else if (d == UP){
        _v->setValueAtIndex(val, i, j+1, k);
    }else  if (d == FORWARD){
        _w->setValueAtIndex(val, i, j, k);
    }else{
        //Backward
        _w->setValueAtIndex(val, i, j, k+1);
    }
}

void MACGrid::addValueAtFace(double val,const int i, const int j, const int k, DirectionEnums d){
    if (d == RIGHT){
        _u->addValueAtIndex(val, i+1, j, k);
    }else if (d == LEFT){
        _u->addValueAtIndex(val, i, j, k);
    }else if (d == DOWN) {
        _v->addValueAtIndex(val, i, j, k);
    }else if (d == UP){
        _v->addValueAtIndex(val, i, j+1, k);
    }else  if (d == FORWARD){
        _w->addValueAtIndex(val, i, j, k);
    }else{
        //Backward
        _w->addValueAtIndex(val, i, j, k+1);
    }
}

/*
void MACGrid::advect(double dt, GridField<int > &cellType){
    
    for (GridFieldIterator<double> iter = _u->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        _u->indexToWorld(i, j, k, x, y, z);
        double val;
        if (cellType.valueAtWorld(x, y, z) == BLUECORE) {
            val = _advect->advect(dt, *this, *_u, i, j, k);
        }else{
            val = 0;// _u->valueAtIndex(iter.index());
        }
        
        buffer()->_u->setValueAtIndex(val, i, j, k);

    }
    
    for (GridFieldIterator<double> iter = _v->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        _v->indexToWorld(i, j, k, x, y, z);
        double val;
        if (cellType.valueAtWorld(x, y, z) == BLUECORE) {
            val = _advect->advect(dt, *this, *_v, i, j, k);
        }else{
            val = 0;//_v->valueAtIndex(iter.index());
        }
        
        buffer()->_v->setValueAtIndex(val, i, j, k);

    }
    
    for (GridFieldIterator<double> iter = _w->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        _w->indexToWorld(i, j, k, x, y, z);
        double val;
        if (cellType.valueAtWorld(x, y, z) == BLUECORE) {
            val = _advect->advect(dt, *this, *_w, i, j, k);
        }else{
            val = 0;//_w->valueAtIndex(iter.index());
        }
        
        buffer()->_w->setValueAtIndex(val, i, j, k);

    }
    
    swapBuffer();
}*/



void MACGrid::addForce(Vector3 vec, double dt){
    
    #pragma omp parallel for
    for (int index = 0; index < 3; index++) {
        GridField<double> *field;
        if (index == 0) {
            field = _u;
        }else if (index == 1){
            field = _v;
        }else{
            field = _w;
        }
        
        for (GridFieldIterator<double> iter = field->iterator(); !iter.done(); iter.next()) {
            int i,j,k;
            iter.index(i, j, k);
            double x,y,z;
            field->indexToWorld(i, j, k, x, y, z);
            
            if (index == 0) {
                double val = _u->valueAtIndex(i, j, k);
                _u->setValueAtIndex(val+vec.x*dt, i, j, k);
            }else if (index == 1){
                double val = _v->valueAtIndex(i, j, k);
                _v->setValueAtIndex(val+vec.y*dt, i, j, k);
            }else{
                double val = _w->valueAtIndex(i, j, k);
                _w->setValueAtIndex(val+vec.z*dt, i, j, k);
            }
        }
    }
    
}

void MACGrid::extrapolate(double dt, GridField<int > &cellType){
    assert(*this == cellType);
    
    for (GridFieldIterator<int> it = cellType.iterator(); !it.done(); it.next()) {
        int i,j,k;
        it.index(i, j, k);
        /*
        if (cellType.valueAtIndex(i,j,k) == SOLID) {
            setValueAtFace(0, i, j, k, LEFT);
            setValueAtFace(0, i, j, k, RIGHT);
            setValueAtFace(0, i, j, k, UP);
            setValueAtFace(0, i, j, k, DOWN);

        }
        
        */
        
        if (k == 0) {
            if (cellType.valueAtIndex(i, j, k) == SOLID){
                    setValueAtFace(0, i, j, k, LEFT);
                    setValueAtFace(0, i, j, k, RIGHT);
                    setValueAtFace(0, i, j, k, UP);
                    setValueAtFace(0, i, j, k, DOWN);
            }else if (cellType.valueAtIndex(i, j, k) == AIR){
                
                if (cellType.valueAtIndex(i-1,j,k) == AIR) {
                    setValueAtFace(0, i, j, k, LEFT);
                }
                
                if (cellType.valueAtIndex(i+1,j,k) == AIR) {
                    setValueAtFace(0, i, j, k, RIGHT);
                }
                
                if (cellType.valueAtIndex(i,j+1,k) == AIR) {
                    setValueAtFace(0, i, j, k, UP);
                }
                
                if (cellType.valueAtIndex(i,j-1,k) == AIR) {
                    setValueAtFace(0, i, j, k, DOWN);
                }
            }
        }
    }
    
    for (GridFieldIterator<int> it = cellType.iterator(); !it.done(); it.next()) {
        int i,j,k;
        it.index(i, j, k);
        
        if (k == 0) {
            if (cellType.valueAtIndex(i, j, k) == IGNITED){
                
                if (cellType.valueAtIndex(i-1,j,k) == AIR) {
                    setValueAtFace(valueAtFace(i, j, k, LEFT), i-1, j, k, LEFT);
                }
                
                if (cellType.valueAtIndex(i+1,j,k) == AIR) {
                    setValueAtFace(valueAtFace(i, j, k, RIGHT), i+1, j, k, RIGHT);
                }
                
                if (cellType.valueAtIndex(i,j+1,k) == AIR) {
                    setValueAtFace(valueAtFace(i, j, k, UP), i, j+1, k, UP);
                }
                
                if (cellType.valueAtIndex(i,j-1,k) == AIR) {
                    setValueAtFace(valueAtFace(i, j, k, DOWN), i, j-1, k, DOWN);
                }
            }
        }
    }
}

void MACGrid::extrapolate3D(double dt, GridField<int > &cellType){
    assert(*this == cellType);
    
    for (GridFieldIterator<int> it = cellType.iterator(); !it.done(); it.next()) {
        int i,j,k;
        it.index(i, j, k);

            if (cellType.valueAtIndex(i, j, k) == SOLID){
                setValueAtFace(0, i, j, k, LEFT);
                setValueAtFace(0, i, j, k, RIGHT);
                setValueAtFace(0, i, j, k, UP);
                setValueAtFace(0, i, j, k, DOWN);
                setValueAtFace(0, i, j, k, FORWARD);
                setValueAtFace(0, i, j, k, BACKWARD);
            }else if (cellType.valueAtIndex(i, j, k) == AIR){
                
                if (cellType.valueAtIndex(i-1,j,k) == AIR) {
                    setValueAtFace(0, i, j, k, LEFT);
                }
                
                if (cellType.valueAtIndex(i+1,j,k) == AIR) {
                    setValueAtFace(0, i, j, k, RIGHT);
                }
                
                if (cellType.valueAtIndex(i,j+1,k) == AIR) {
                    setValueAtFace(0, i, j, k, UP);
                }
                
                if (cellType.valueAtIndex(i,j-1,k) == AIR) {
                    setValueAtFace(0, i, j, k, DOWN);
                }
                
                if (cellType.valueAtIndex(i,j,k+1) == AIR) {
                    setValueAtFace(0, i, j, k, FORWARD);
                }
                
                if (cellType.valueAtIndex(i,j,k-1) == AIR) {
                    setValueAtFace(0, i, j, k, BACKWARD);
                }
            }else if(cellType.valueAtIndex(i, j, k) == SOLID){
                setValueAtFace(0, i, j, k, UP);
                setValueAtFace(0, i, j, k, DOWN);
                setValueAtFace(0, i, j, k, LEFT);
                setValueAtFace(0, i, j, k, RIGHT);
                setValueAtFace(0, i, j, k, FORWARD);
                setValueAtFace(0, i, j, k, BACKWARD);
                
            }
        
    }
    
    for (GridFieldIterator<int> it = cellType.iterator(); !it.done(); it.next()) {
        int i,j,k;
        it.index(i, j, k);

            if (cellType.valueAtIndex(i, j, k) == IGNITED){
                
                if (cellType.valueAtIndex(i-1,j,k) == AIR) {
                    setValueAtFace(valueAtFace(i, j, k, LEFT), i-1, j, k, LEFT);
                }
                
                if (cellType.valueAtIndex(i+1,j,k) == AIR) {
                    setValueAtFace(valueAtFace(i, j, k, RIGHT), i+1, j, k, RIGHT);
                }
                
                if (cellType.valueAtIndex(i,j+1,k) == AIR) {
                    setValueAtFace(valueAtFace(i, j, k, UP), i, j+1, k, UP);
                }
                
                if (cellType.valueAtIndex(i,j-1,k) == AIR) {
                    setValueAtFace(valueAtFace(i, j, k, DOWN), i, j-1, k, DOWN);
                }
                
                if (cellType.valueAtIndex(i,j,k+1) == AIR) {
                    setValueAtFace(valueAtFace(i, j, k, FORWARD), i, j, k+1, FORWARD);
                }
                
                if (cellType.valueAtIndex(i,j,k-1) == AIR) {
                    setValueAtFace(valueAtFace(i, j, k, BACKWARD), i, j, k-1, BACKWARD);
                }
                
                
            }
        }
    
}












