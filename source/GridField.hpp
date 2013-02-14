//
//  GridField.cpp
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/14/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#include "GridField.h"

template<class T>
GridField<T>::GridField(int xdim,int ydim, int zdim):_xdim(xdim),_ydim(ydim),_zdim(zdim){
    data = new T[xdim*ydim*zdim];
}

template<class T>
GridField<T>::~GridField(){
    delete [] data;
}
//Private
template<class T>
inline int GridField<T>::indexAt(int i, int j, int k) const{
    return i+_ydim*j+_xdim*_ydim*k;
}

template<class T>
inline int GridField<T>::indexAt(int index,int &i, int &j, int &k) const{
    i = index % _xdim;
    j = index % _ydim;
}

//Transformera världskoordinater till lokala koordinater
template<class T>
void GridField<T>::worldToLocal(const double w_x,const double w_y,const double w_z, double &l_x,double &l_y,double &l_z) const{
    l_x = itrans[0]*w_x+itrans[1]*w_y+itrans[2]*w_z+itrans[3];
    l_y = itrans[4]*w_x+itrans[5]*w_y+itrans[6]*w_z+itrans[7];
    l_z = itrans[8]*w_x+itrans[9]*w_y+itrans[10]*w_z+itrans[11];
}

//Transformera lokalakoordinater till index
template<class T>
int GridField<T>::localToIndex(const double l_x,const double l_y,const double l_z) const{
    //Index
    return indexAt(floor(l_x/_dx), floor(l_y/_dx), floor(l_z/_dx) );
}

//Public
template<class T>
void GridField<T>::setTransformation(double *m){ //4x4 matrix som transformerar cellkoordinater till världskoordinater
    trans = m;
    //Gör invers på m!
    //itrans = inverse(m);
}

template<class T>
inline T GridField<T>::valueAtIndex(int i) const{
    return data[i];
}

template<class T>
inline T GridField<T>::valueAtIndex(int i,int j,int k) const{
    return valueAtIndex(indexAt(i, j, k));
}

template<class T>
inline T GridField<T>::valueAtWorld(double w_x, double w_y,double w_z) const{
    double l_x,l_y,l_z;
    worldToLocal(w_x,w_y,w_z, l_x,l_y,l_z);  //Konverera till lokala koordinater
    int index = localToIndex(l_x, l_y, l_z); //Lokala koordinater till index
    //Interpolera...
    
    
    return valueAtIndex(index);
}

template<class T>
inline void GridField<T>::cellToWorld(int i,int j,int k, double &w_x, double &w_y,double &w_z) const{
    
}

template<class T>
inline void GridField<T>::worldToCell(int &i,int &j,int &k, double &w_x, double &w_y,double &w_z) const{
    
}







