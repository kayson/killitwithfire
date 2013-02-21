//
//  GridMapping.cpp
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/17/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#include "GridMapping.h"

#ifdef __APPLE__
#include "transform.hpp"
#elif defined _WIN32 || defined _WIN64
#include <glm/gtx/transform.hpp>
#define round(x) floor((x) >= 0 ? (x) + 0.5 : (x) - 0.5)
#endif

//Konstruktor
GridMapping::GridMapping():_xdim(1),_ydim(1),_zdim(1),_size(_xdim*_ydim*_zdim){
    _trans = glm::dmat4(1.0); //Enhetsmatris
    _itrans = glm::inverse(_trans);
}

GridMapping::GridMapping(unsigned int dim, glm::dmat4x4 t):_xdim(dim),_ydim(dim),_zdim(dim),_size(_xdim*_ydim*_zdim){
    _trans = t;
    _itrans = glm::inverse(_trans);
}

GridMapping::GridMapping(unsigned int xdim,unsigned int ydim,unsigned int zdim):_xdim(xdim),_ydim(ydim),_zdim(zdim),_size(_xdim*_ydim*_zdim){
    _trans = glm::dmat4(1.0); //Enhetsmatris
    _itrans = glm::inverse(_trans);

}
GridMapping::GridMapping(unsigned int xdim,unsigned int ydim,unsigned int zdim, glm::dmat4x4 t):_xdim(xdim),_ydim(ydim),_zdim(zdim),_size(_xdim*_ydim*_zdim){
    _trans = t;
    _itrans = glm::inverse(_trans);

}

GridMapping::GridMapping(unsigned int xdim,unsigned int ydim,unsigned int zdim, double size):_xdim(xdim),_ydim(ydim),_zdim(zdim),_size(_xdim*_ydim*_zdim){
    _trans = glm::scale(size, size, size);
    _itrans = glm::inverse(_trans);
}

GridMapping::GridMapping(const GridMapping &g){
    _trans = g._trans;
    _itrans = g._itrans;
    _xdim = g._xdim;
    _ydim = g._ydim;
    _zdim = g._zdim;
    _size = g._size;

}

GridMapping::~GridMapping(){
    
}

GridMapping & GridMapping::operator= (const GridMapping & other){
    if (this != &other){
        _trans = other._trans;
        _itrans = other._itrans;
        _xdim = other.xdim();
        _ydim = other.ydim();
        _zdim = other.zdim();
        _size = other._size;
    }
    return *this;
}

//Transformation
void GridMapping::setTransformation(glm::dmat4x4 t){
    _trans = glm::dmat4x4(t);
    _itrans = glm::inverse(_trans);
}

void GridMapping::multTransformation(glm::dmat4x4 t){
    _trans = glm::dmat4x4(_trans*t);
    _itrans = glm::inverse(_trans);
}

//Local dim
double GridMapping::local_dx() const{
    return 1.0/xdim();
}
double GridMapping::local_dy() const{
    return 1.0/ydim();
}
double GridMapping::local_dz() const{
        return 1.0/zdim();
}
//Dim
unsigned int GridMapping::xdim() const{
    return _xdim;
}
unsigned int GridMapping::ydim() const{
    return _ydim;
}
unsigned int GridMapping::zdim() const{
    return _zdim;
}
unsigned int GridMapping::size() const{
    return _size;
}

double GridMapping::dx() const{
    if (xdim() != 0) {
        double x1,x0,y,z;
        indexToWorld(0, 0, 0, x0, y, z);
        indexToWorld(1, 0, 0, x1, y, z);
        return fabs(x1-x0);
    }
    return 0;
}

double GridMapping::dy() const{
    if (ydim() != 0) {
        double x,y0,y1,z;
        indexToWorld(0, 0, 0, x, y0, z);
        indexToWorld(0, 1, 0, x, y1, z);
        return fabs(y1-y0);
    }
    return 0;
}

double GridMapping::dz() const{
    if (ydim() != 0) {
        double x,y,z0,z1;
        indexToWorld(0, 0, 0, x, y, z0);
        indexToWorld(0, 0, 1, x, y, z1);
        return fabs(z1-z0);
    }
    return 0;
}

//Funktioner
//Index to index
 int GridMapping::indexAt(int i, int j, int k) const{
    return i+(xdim())*j+(xdim())*(ydim())*k;
}

void GridMapping::indexAt(int index, int &i, int &j, int &k) const{
    k = index /(xdim()*ydim());
    j = (index-(k*xdim()*ydim()))/(xdim());
    i = (index-j*xdim()-k*xdim()*ydim());
}

//Index to...
void GridMapping::indexToWorld(int i,int j,int k, double &w_x, double &w_y,double &w_z) const{
    double l_x,l_y,l_z;
    indexToLocal(i, j, k, l_x, l_y, l_z);
    localToWorld(l_x, l_y, l_z, w_x, w_y, w_z);
}
void GridMapping::indexToLocal(int i,int j,int k, double &l_x, double &l_y,double &l_z) const{
    l_x = ((double)i)/((double)xdim());
    l_y = ((double)j)/((double)ydim());
    l_z = ((double)k)/((double)zdim());
}

//Local to...
void GridMapping::localToIndex(double l_x, double l_y,double l_z,int &i,int &j,int &k) const{
    i = round(l_x*((double)xdim()));
    j = round(l_y*((double)ydim()));
    k = round(l_z*((double)zdim()));
}

void GridMapping::localToWorld(double l_x, double l_y,double l_z, double &w_x,double &w_y,double &w_z) const{
    w_x = l_x*_trans[0][0]+l_y*_trans[1][0]+l_z*_trans[2][0]+_trans[3][0];
    w_y = l_x*_trans[0][1]+l_y*_trans[1][1]+l_z*_trans[2][1]+_trans[3][1];
    w_z = l_x*_trans[0][2]+l_y*_trans[1][2]+l_z*_trans[2][2]+_trans[3][2];
}

void GridMapping::localToUpperLeftIndex(const double l_x,const double l_y,const double l_z, int &i,int &j,int &k) const{
    i = floor(l_x*((double)xdim()));
    j = floor(l_y*((double)ydim()));
    k = floor(l_z*((double)zdim()));
}

//World to...
void GridMapping::worldToLocal(const double w_x,const double w_y,const double w_z, double &l_x,double &l_y,double &l_z) const{
    l_x = w_x*_itrans[0][0]+w_y*_itrans[1][0]+w_z*_itrans[2][0]+_itrans[3][0];
    l_y = w_x*_itrans[0][1]+w_y*_itrans[1][1]+w_z*_itrans[2][1]+_itrans[3][1];
    l_z = w_x*_itrans[0][2]+w_y*_itrans[1][2]+w_z*_itrans[2][2]+_itrans[3][2];
}
void GridMapping::worldToIndex(int &i,int &j,int &k, double w_x, double w_y,double w_z) const{
    double l_x,l_y,l_z;
    worldToLocal(w_x, w_y, w_z, l_x, l_y, l_z);
    localToIndex(l_x, l_y, l_z, i, j, k);
}
void GridMapping::worldToUpperLeftIndex(const double w_x,const double w_y,const double w_z, int &i,int &j,int &k) const{
    double l_x,l_y,l_z;
    worldToLocal(w_x, w_y, w_z, l_x, l_y, l_z);
    localToUpperLeftIndex(l_x, l_y, l_z, i, j, k);
}
GridMappingIterator GridMapping::iterator() const{
    return GridMappingIterator(this);
}

