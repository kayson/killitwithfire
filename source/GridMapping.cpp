//
//  GridMapping.cpp
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/17/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#include "GridMapping.h"

#ifdef __APPLE__
#elif defined _WIN32 || defined _WIN64
#define round(x) floor((x) >= 0 ? (x) + 0.5 : (x) - 0.5)
#endif

//Konstruktor
GridMapping::GridMapping(){
    _trans = glm::mat4x4(1,0,0,0,
                         0,1,0,0,
                         0,0,1,0,
                         0,0,0,1);
    _itrans = glm::inverse(_trans);
    _xdim = 1;
    _ydim = 1;
    _zdim = 1;
}

GridMapping::GridMapping(int dim, glm::mat4x4 t):_xdim(dim),_ydim(dim),_zdim(dim){
    _trans = t;
    _itrans = glm::inverse(_trans);
}

GridMapping::GridMapping(int xdim,int ydim,int zdim):_xdim(xdim),_ydim(ydim),_zdim(zdim){
    _trans = glm::mat4x4(1,0,0,0,
                         0,1,0,0,
                         0,0,1,0,
                         0,0,0,1);
    _itrans = glm::inverse(_trans);
}
GridMapping::GridMapping(int xdim,int ydim,int zdim, glm::mat4x4 t):_xdim(xdim),_ydim(ydim),_zdim(zdim){
    _trans = t;
    _itrans = glm::inverse(_trans);
}


GridMapping::GridMapping(const GridMapping &g){
    _trans = g._trans;
    _itrans = g._itrans;
    _xdim = g.xdim();
    _ydim = g.ydim();
    _zdim = g.zdim();
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
    }
    return *this;
}

//Transformation
void GridMapping::setTransformation(double *t){
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            _trans[i][j] = t[i+j*4];
        }
    }
    _itrans = glm::inverse(_trans);
}

void GridMapping::setTransformation(glm::mat4x4 t){
    _trans = t;
    _itrans = glm::inverse(_trans);
}

//Dim
int GridMapping::xdim() const{
    return _xdim;
}
int GridMapping::ydim() const{
    return _ydim;
}
int GridMapping::zdim() const{
    return _zdim;
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
    l_x = ((double)i)/((double)xdim()-1.0);
    l_y = ((double)j)/((double)ydim()-1.0);
    l_z = ((double)k)/((double)zdim()-1.0);
}

//Local to...
void GridMapping::localToIndex(double l_x, double l_y,double l_z,int &i,int &j,int &k) const{
    i = round(l_x*((double)xdim()-1.0));
    j = round(l_y*((double)ydim()-1.0));
    k = round(l_z*((double)zdim()-1.0));
}

void GridMapping::localToWorld(double l_x, double l_y,double l_z, double &w_x,double &w_y,double &w_z) const{
    w_x = _trans[0][0]*l_x+_trans[1][0]*l_y+_trans[2][0]*l_z+_trans[3][0];
    w_y = _trans[0][1]*l_x+_trans[1][1]*l_y+_trans[2][1]*l_z+_trans[3][1];
    w_z = _trans[0][2]*l_x+_trans[1][2]*l_y+_trans[2][2]*l_z+_trans[3][2];
}

void GridMapping::localToUpperLeftIndex(const double l_x,const double l_y,const double l_z, int &i,int &j,int &k) const{
    i = floor(l_x*((double)xdim()-1.0));
    j = floor(l_y*((double)ydim()-1.0));
    k = floor(l_z*((double)zdim()-1.0));
}

//World to...
void GridMapping::worldToLocal(const double w_x,const double w_y,const double w_z, double &l_x,double &l_y,double &l_z) const{
    l_x = _itrans[0][0]*w_x+_itrans[1][0]*w_y+_itrans[2][0]*w_z+_itrans[3][0];
    l_y = _itrans[0][1]*w_x+_itrans[1][1]*w_y+_itrans[2][1]*w_z+_itrans[3][1];
    l_z = _itrans[0][2]*w_x+_trans[1][2]*w_y+_itrans[2][2]*w_z+_itrans[3][2];
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

