//
//  GridField.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/14/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#include "Vector3.h"
#include <glm/glm.hpp>

#ifndef FuidFire_GridField_h
#define FuidFire_GridField_h

template <class T>

class GridField {
private:
    T *data;
    const int _xdim,_ydim, _zdim;
    double *trans,*itrans;
    double _dx;
private:
    //Access values
    inline int indexAt(int i, int j, int k) const;
    inline int indexAt(int index,int &i, int &j, int &k) const;
    void worldToLocal(const double w_x,const double w_y,const double w_z, double &l_x,double &l_y,double &l_z) const;
    int localToIndex(const double l_x,const double l_y,const double l_z) const;

public:
    GridField(int xdim,int ydim, int zdim):_xdim(xdim),_ydim(ydim),_zdim(zdim){ data = new T[xdim*ydim*zdim]; };

    ~GridField();
    void setTransformation(double *m); //4x4 matrix som transformerar cellkoordinater till världskoordinater 

    inline T valueAtIndex(int i) const;
    inline T valueAtIndex(int i,int j,int k) const;
    inline T valueAtWorld(double w_x, double w_y,double w_z) const;
    inline void cellToWorld(int i,int j,int k, double &w_x, double &w_y,double &w_z) const;
    inline void worldToCell(int &i,int &j,int &k, double &w_x, double &w_y,double &w_z) const;
    
};

#endif
