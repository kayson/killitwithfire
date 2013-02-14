//
//  GridField.cpp
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/14/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#include "GridField.h"

//template<class T>
//GridField<T>::GridField(int xdim,int ydim, int zdim)

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







