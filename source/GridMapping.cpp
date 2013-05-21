//
//  GridMapping.cpp
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/17/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#include "GridMapping.h"

#if defined __APPLE__
//#include "transform.hpp"
#include "transform.hpp"
#elif defined __unix__
#include "glm/gtx/transform.hpp"
#elif defined _WIN32 || defined _WIN64
#include <glm/gtx/transform.hpp>
#define round(x) floor((x) >= 0 ? (x) + 0.5 : (x) - 0.5)
#endif

//Konstruktor
GridMapping::GridMapping():_xdim(1),_ydim(1),_zdim(1),_size(_xdim*_ydim*_zdim){
    _fast_trans = new double[16];
    _fast_itrans = new double[16];
    setTransformation(glm::dmat4(1.0));
}

GridMapping::GridMapping(unsigned int dim, glm::dmat4x4 t):_xdim(dim),_ydim(dim),_zdim(dim),_size(_xdim*_ydim*_zdim){
    _fast_trans = new double[16];
    _fast_itrans = new double[16];
    setTransformation(t);
}

GridMapping::GridMapping(unsigned int xdim,unsigned int ydim,unsigned int zdim):_xdim(xdim),_ydim(ydim),_zdim(zdim),_size(_xdim*_ydim*_zdim){
    _fast_trans = new double[16];
    _fast_itrans = new double[16];
    setTransformation(glm::dmat4(1.0));

}
GridMapping::GridMapping(unsigned int xdim,unsigned int ydim,unsigned int zdim, glm::dmat4x4 t):_xdim(xdim),_ydim(ydim),_zdim(zdim),_size(_xdim*_ydim*_zdim){
    _fast_trans = new double[16];
    _fast_itrans = new double[16];
    setTransformation(t);
}

GridMapping::GridMapping(unsigned int xdim,unsigned int ydim,unsigned int zdim, double size):_xdim(xdim),_ydim(ydim),_zdim(zdim),_size(_xdim*_ydim*_zdim){
    _fast_trans = new double[16];
    _fast_itrans = new double[16];
    setTransformation(glm::scale(size, size, size));
}

GridMapping::GridMapping(const GridMapping &g){
    _fast_trans = new double[16];
    _fast_itrans = new double[16];
    setTransformation(g._trans);
    _xdim = g._xdim;
    _ydim = g._ydim;
    _zdim = g._zdim;
    _size = g._size;


}

GridMapping::~GridMapping(){
    
}

GridMapping & GridMapping::operator= (const GridMapping & other){
    if (this != &other){
        _fast_trans = new double[16];
        _fast_itrans = new double[16];
        setTransformation(other._trans);
        _itrans = other._itrans;
        _xdim = other.xdim();
        _ydim = other.ydim();
        _zdim = other.zdim();
        _size = other._size;
    }
    return *this;
}

bool GridMapping::operator== (const GridMapping & other){
    
    
    return (other.xdim() == xdim() &&
            other.ydim() == ydim() &&
            other.zdim() == zdim() &&
            other._trans == _trans &&
            other._itrans == _itrans &&
            other.size() == size());
}

//Transformation
void GridMapping::setTransformation(glm::dmat4x4 t){
    _trans = glm::dmat4x4(t);
    _itrans = glm::inverse(_trans);
    /*w_x = l_x*_trans[0][0]+l_y*_trans[1][0]+l_z*_trans[2][0]+_trans[3][0];
    w_y = l_x*_trans[0][1]+l_y*_trans[1][1]+l_z*_trans[2][1]+_trans[3][1];
    w_z = l_x*_trans[0][2]+l_y*_trans[1][2]+l_z*_trans[2][2]+_trans[3][2];
     */
    for (int i = 0;  i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            _fast_itrans[i+j*4] = _itrans[i][j];
            _fast_trans[i+j*4] = _trans[i][j];
        }
    }
}

void GridMapping::multTransformation(glm::dmat4x4 t){
    _trans = glm::dmat4x4(_trans*t);
    _itrans = glm::inverse(_trans);
    
    for (int i = 0;  i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            _fast_itrans[i+j*4] = _itrans[i][j];
            _fast_trans[i+j*4] = _trans[i][j];
        }
    }
}

glm::dmat4x4 GridMapping::getTrans() const{
    return _trans;
}

//Local dim
double GridMapping::local_dx() const{
    return 1.0/double(xdim()-1);
}
double GridMapping::local_dy() const{
    return 1.0/double(ydim()-1);
}
double GridMapping::local_dz() const{
	return 1.0/double(zdim()-1);
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
int GridMapping::size() const{
    return _size;
}

double GridMapping::dx() const{
    if (xdim() != 0) {
        double x1,x0,y,z;
        indexToWorld(0, 0, 0, x0, y, z);
        indexToWorld(1, 0, 0, x1, y, z);
        return abs(x1-x0);
    }
    return 0;
}

double GridMapping::dy() const{
    if (ydim() != 0) {
        double x,y0,y1,z;
        indexToWorld(0, 0, 0, x, y0, z);
        indexToWorld(0, 1, 0, x, y1, z);
        return abs(y1-y0);
    }
    return 0;
}

double GridMapping::dz() const{
    if (ydim() != 0) {
        double x,y,z0,z1;
        indexToWorld(0, 0, 0, x, y, z0);
        indexToWorld(0, 0, 1, x, y, z1);
        return abs(z1-z0);
    }
    return 0;
}

//Funktioner
//Index to index
 int GridMapping::indexAt(int i, int j, int k) const{
    return i+(xdim())*j+(xdim()*ydim()*k);
}

void GridMapping::indexAt(int index, int &i, int &j, int &k) const{
    k = index /(xdim()*ydim());
    j = (index-(k*xdim()*ydim()))/(xdim());
    i = (index-j*xdim()-k*xdim()*ydim());
}

bool GridMapping::isValid(int i, int j, int k) const{
    return (   i >= 0 && i < xdim()
            && j >= 0 && j < ydim()
            && k >= 0 && k < zdim());
}

//Index to...
void GridMapping::indexToWorld(int i,int j,int k, double &w_x, double &w_y,double &w_z) const{
    double l_x,l_y,l_z;
    indexToLocal(i, j, k, l_x, l_y, l_z);
    localToWorld(l_x, l_y, l_z, w_x, w_y, w_z);
    
    /*
     l_x = ((double)i)/((double)xdim());
     l_y = ((double)j)/((double)ydim());
     l_z = ((double)k)/((double)zdim());
     
     w_x = l_x*_trans[0][0]+l_y*_trans[1][0]+l_z*_trans[2][0]+_trans[3][0];
     w_y = l_x*_trans[0][1]+l_y*_trans[1][1]+l_z*_trans[2][1]+_trans[3][1];
     w_z = l_x*_trans[0][2]+l_y*_trans[1][2]+l_z*_trans[2][2]+_trans[3][2];
     
    */
    
}
void GridMapping::indexToLocal(int i,int j,int k, double &l_x, double &l_y,double &l_z) const{
    l_x = double(i)/double(xdim()-1);
    l_y = double(j)/double(ydim()-1);
    l_z = double(k)/double(zdim()-1);//TODO om zdim = 1 så funkar inte detta, en ifsats kan lösa detta
}

//Local to...
void GridMapping::localToIndex(double l_x, double l_y,double l_z,int &i,int &j,int &k) const{
    i = (int)round(l_x*double(xdim()-1));
    j = (int)round(l_y*double(ydim()-1));
    k = (int)round(l_z*double(zdim()-1));
}

void GridMapping::localToWorld(double l_x, double l_y,double l_z, double &w_x,double &w_y,double &w_z) const{
    w_x = l_x*_fast_trans[0]+l_y*_fast_trans[1]+l_z*_fast_trans[2]+_fast_trans[3];
    w_y = l_x*_fast_trans[4]+l_y*_fast_trans[5]+l_z*_fast_trans[6]+_fast_trans[7];
    w_z = l_x*_fast_trans[8]+l_y*_fast_trans[9]+l_z*_fast_trans[10]+_fast_trans[11];
}

void GridMapping::localToUpperLeftIndex(const double l_x,const double l_y,const double l_z, int &i,int &j,int &k) const{
	i = (int)floor(l_x*double(xdim()-1));
    j = (int)floor(l_y*double(ydim()-1));
    k = (int)floor(l_z*double(zdim()-1));
}

//World to...
void GridMapping::worldToLocal(const double w_x,const double w_y,const double w_z, double &l_x,double &l_y,double &l_z) const{
    l_x = w_x*_fast_itrans[0]+w_y*_fast_itrans[1]+w_z*_fast_itrans[2]+_fast_itrans[3];
    l_y = w_x*_fast_itrans[4]+w_y*_fast_itrans[5]+w_z*_fast_itrans[6]+_fast_itrans[7];
    l_z = w_x*_fast_itrans[8]+w_y*_fast_itrans[9]+w_z*_fast_itrans[10]+_fast_itrans[11];
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

