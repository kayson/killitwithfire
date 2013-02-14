//
//  GridField.cpp
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/14/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#include "GridField.h"
#include <algorithm>
#include "Interpolation.h"
<<<<<<< HEAD
=======
#include <cmath>
>>>>>>> Småfix

template<class T>
GridField<T>::GridField():_xdim(1),_ydim(1),_zdim(1){
    //Cell data
    _data = new T[cellCount()];
    std::fill(_data,_data+cellCount(),0);
    //Använd Enhetsmatris
    double *m = new double[4*4];
    std::fill(m, m+4*4, 0);
    m[0] = 1; m[5] = 1; m[10] = 1; m[15] = 1.0;
    setTransformation(m);
}

<<<<<<< HEAD
//template<class T>
//GridField<T>::GridField(int xdim,int ydim, int zdim)
=======
template<class T>
GridField<T>::GridField(int xdim,int ydim, int zdim):_xdim(xdim),_ydim(ydim),_zdim(zdim){
    //Cell data
    _data = new T[cellCount()];
    std::fill(_data,_data+cellCount(),0);
    
    //Sätt Enhetsmatris
    double *m = new double[4*4];
    std::fill(m, m+4*4, 0);
    m[0] = 1; m[5] = 1; m[10] = 1; m[15] = 1.0;
    setTransformation(m);
}
>>>>>>> f181c730ef9dc1999648c397eff6bce63743be8d

template<class T>
GridField<T>::~GridField(){
    delete [] _data;
}

//Private
template<class T>
inline int GridField<T>::indexAt(int i, int j, int k) const{
    return i+_ydim*j+_xdim*_ydim*k;
}

template<class T>
inline void GridField<T>::indexAt(int index,int &i, int &j, int &k) const{
    k = index /(_xdim*_ydim);
    j = (index-(k*_xdim*_ydim))/(_ydim);
    i = (index-j*_ydim-k*_xdim*_ydim);
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
<<<<<<< HEAD
    return indexAt( floor(l_x/((double)_xdim) + 0.5) , floor(l_y/((double)_ydim) + 0.5) , floor(l_z/((double)_zdim)) + 0.5);
=======
    return indexAt( round(l_x*((double)_xdim)) , round(l_y*((double)_ydim)) , round(l_z*((double)_zdim)));
>>>>>>> Småfix
}

template<class T>
int GridField<T>::localToUpperLeftIndex(const double l_x,const double l_y,const double l_z) const{
    //Index
    return indexAt( floor(l_x*((double)_xdim)) , floor(l_y*((double)_ydim)) , floor(l_z*((double)_zdim)));
}

//Public
template<class T>
void GridField<T>::setTransformation(double *m){ //4x4 matrix som transformerar cellkoordinater till världskoordinater
    trans = m;

	glm::mat4x4 temp(m[0],m[1],m[2],m[3],
					m[4],m[5],m[6],m[7],
					m[8],m[9],m[10],m[11],
					m[12],m[13],m[14],m[15]);
	temp = glm::inverse(temp);

    itrans = new double[16];
	itrans[0] = temp[0][0];itrans[1] = temp[0][1];itrans[2] = temp[0][2];itrans[3] = temp[0][3];
	itrans[4] = temp[1][0];itrans[5] = temp[1][1];itrans[6] = temp[1][2];itrans[7] = temp[1][3];
	itrans[8] = temp[2][0];itrans[9] = temp[2][1];itrans[10] = temp[2][2];itrans[11] = temp[2][3];
	itrans[12] = temp[3][0];itrans[13] = temp[3][1];itrans[14] = temp[3][2];itrans[15] = temp[3][3];

}

template<class T>
inline T GridField<T>::valueAtIndex(int i) const{
    return _data[i];
}

template<class T>
inline T GridField<T>::valueAtIndex(int i,int j,int k) const{
    return valueAtIndex(indexAt(i, j, k));
}

template<class T>
inline T GridField<T>::valueAtWorld(double w_x, double w_y,double w_z) const{
    int index;
    int i,j,k;
    double l_x,l_y,l_z;
    double cell_x,cell_y,cell_z;

    //Konvertera världskoordinater till cellkoordinater
    worldToLocal(w_x, w_y, w_z, l_x, l_y, l_z);
    //Konverera lokala koordinater till 1D index för cellen närmast "upp" til vänster
    index = localToUpperLeftIndex(l_x, l_y, l_z);
    //Konvertera 1D index till 3D index (i,j,k)
    indexAt(index, i, j, k);
    
    //Konvertera lokala koordinater för index till cellkoordinater (0..1)
    localToCellCoordinate(i,j,k,l_x,l_y,l_x,cell_x,cell_y,cell_z);
    
    //Interpolera...
    LinearInterpolation<T> interpolation = LinearInterpolation<T>();
    T t1 = interpolation(cell_x, valueAtIndex(i,j,k),valueAtIndex(i+1,j,k));
    T t2 = interpolation(cell_x,valueAtIndex(i,j+1,k),valueAtIndex(i+1,j+1,k));
    T t3 = interpolation(cell_x,valueAtIndex(i,j+1,k+1),valueAtIndex(i+1,j+1,k+1));
    T t4 = interpolation(cell_x,valueAtIndex(i,j+1,k+1),valueAtIndex(i+1,j+1,k+1));

    T t5 = interpolation(cell_y,t1,t2);
    T t6 = interpolation(cell_y,t3,t4);

    T t7 = interpolation(cell_z,t5,t6);
    
    return t7;
}

template<class T>
int GridField<T>::cellCount() const{
    return _xdim*_ydim*_zdim;
}

template<class T>
inline void GridField<T>::cellToWorld(int i,int j,int k, double &w_x, double &w_y,double &w_z) const{
    double x = i,y = j,z = k;
    w_x = trans[0]*x+trans[1]*y+trans[2]*z+trans[3];
    w_y = trans[4]*x+trans[5]*y+trans[6]*z+trans[7];
    w_z = trans[8]*x+trans[9]*y+trans[10]*z+trans[11];
}

template<class T>
inline void GridField<T>::worldToCell(int &i,int &j,int &k, double &w_x, double &w_y,double &w_z) const{
    double l_x = itrans[0]*w_x+itrans[1]*w_y+itrans[2]*w_z+itrans[3];
    double l_y = itrans[4]*w_x+itrans[5]*w_y+itrans[6]*w_z+itrans[7];
    double l_z = itrans[8]*w_x+itrans[9]*w_y+itrans[10]*w_z+itrans[11];
    indexAt(localToIndex(l_x,l_y, l_z),i,j,k);
}


template<class T>
inline void GridField<T>::localToCellCoordinate(int i,int j,int k, double l_x, double l_y,double l_z, double &c_x,double &c_y,double &x_z) const{
    
    indexAt(localToIndex(l_x,l_y, l_z),i,j,k);
}
template<class T>
inline void GridField<T>::setValueAtIndex(T val,int i){
    _data[i] = val;
}
template<class T>
inline void GridField<T>::setValueAtIndex(T val,int i,int j,int k){
    _data[indexAt(i, j, k)] = val;
}

template<class T>
GridFieldIterator<T> GridField<T>::iterator(){
    return GridFieldIterator<T>(this);
}



