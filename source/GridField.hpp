//
//  GridField.cpp
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/14/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#include "GridField.h"
#include "Interpolation.h"
#include <exception>

#include "GridMapping.h"



#ifdef __APPLE__
#elif defined _WIN32 || defined _WIN64
#define round(x) floor((x) >= 0 ? (x) + 0.5 : (x) - 0.5)
#endif

#ifndef FuidFire_GridField_hpp
#define FuidFire_GridField_hpp

template<class T>
GridField<T>::GridField(const GridMapping &m){
    mapping = m;
    
    //Allokera data-array
    _data = new T[cellCount()];
    for (int i = 0; i < cellCount(); i++) _data[i] = T();
}


template<class T>
GridField<T>::GridField():GridField(10,10,10){
}

template<class T>
GridField<T>::GridField(int xdim,int ydim, int zdim){
    //mapping
    mapping = GridMapping(xdim,ydim,zdim);
    
    //Allokera data-array
    _data = new T[cellCount()];
    for (int i = 0; i < cellCount(); i++) _data[i] = T();

}

template<class T>
GridField<T>::GridField(const GridField<T> &g){

    mapping = GridMapping(g.mapping);
    
    _data = new T[g.cellCount()];

    //Iterera över g
    for (GridFieldIterator<T> iter = g.iterator(); !iter.done(); iter.next()) {
        setValueAtIndex(iter.value(), iter.index());
    }
}

template<class T>
GridField<T>& GridField<T>::operator=(const GridField<T> &g){
    if (this != g) {
        mapping = g.mapping;
        
        //Allokera data-array
        _data = new T[cellCount()];
        for (int i = 0; i < cellCount(); i++) _data[i] = T();

        *_extrapolation = *g._extrapolation;

    }
    
    return *this;
}


template<class T>
GridField<T>::~GridField(){
    delete [] _data;
}

template<class T>
T GridField<T>::valueAtIndex(int i) const{
    return _data[i];
}

template<class T>
T GridField<T>::valueAtIndex(int i,int j,int k) const{
    
    if (!(i < xdim() && i >= 0 && j >= 0 && j < ydim() && k >= 0 && k < zdim())) {
		//std::string s = "Index ("+ std::to_string(i) + "," + std::to_string(j) + ","+ std::to_string(k)+") out of bounds [0..";
        //s += std::to_string(xdim()) + "],[0.." + std::to_string(ydim()) + "],[0.." + std::to_string(zdim()) + "]";
        std::string s = "Out of bounds";
		throw std::runtime_error(s);
    }
    return valueAtIndex(mapping.indexAt(i, j, k));
}

template<class T>
T GridField<T>::valueAtWorld(double w_x, double w_y,double w_z) const{
    
    int i,j,k;
    double w_x0,w_y0,w_z0;
    double w_x1,w_y1,w_z1;
    double x,y,z;

    //Konvertera världskoordinater till cellkoordinater
    mapping.worldToUpperLeftIndex(w_x, w_y, w_z, i, j, k);
    mapping.indexToWorld(i, j, k, w_x0, w_y0, w_z0);
    mapping.indexToWorld(i+1, j+1, k+1, w_x1, w_y1, w_z1);

    x = (w_x-w_x0)/(w_x1-w_x0);
    y = (w_y-w_y0)/(w_y1-w_y0);
    z = (w_z-w_z0)/(w_z1-w_z0);
    assert(i < 100);
    //FIXA!!! -->    
    //Interpolera...
    LinearInterpolation<T> interpolation = LinearInterpolation<T>();
    T t1 = interpolation(x,valueAtIndex(i,j,k),valueAtIndex(i+1,j,k));
    T t2 = interpolation(x,valueAtIndex(i,j+1,k),valueAtIndex(i+1,j+1,k));
    T t3 = interpolation(x,valueAtIndex(i,j,k+1),valueAtIndex(i+1,j,k+1));
    T t4 = interpolation(x,valueAtIndex(i,j+1,k+1),valueAtIndex(i+1,j+1,k+1));
    
    T t5 = interpolation(y,t1,t2);
    T t6 = interpolation(y,t3,t4);
    
    T t7 = interpolation(z,t5,t6);
    
    return t7;
}

template<class T>
int GridField<T>::cellCount() const{
    return (mapping.xdim())*(mapping.ydim())*(mapping.zdim());
}
//Dim
template<class T>
int GridField<T>::xdim() const{
    return mapping.xdim();
}
template<class T>
int GridField<T>::ydim() const{
    return mapping.ydim();
}
template<class T>
int GridField<T>::zdim() const{
    return mapping.zdim();
}

template<class T>
int GridField<T>::size() const{
    return mapping.xdim()*mapping.ydim()*mapping.zdim();
}

template<class T>
void GridField<T>::setValueAtIndex(T val,int i){
    _data[i] = val;
}
template<class T>
void GridField<T>::setValueAtIndex(T val,int i,int j,int k){
    int index = mapping.indexAt(i, j, k);
    _data[index] = val;
}

//Operatorer
template<class T>
T GridField<T>::operator()(int i, int j, int k) const{
    return valueAtIndex(i, j, k);
}

template<class T>
const GridFieldIterator<T> GridField<T>::iterator() const{
    return GridFieldIterator<T>(this);
}

#endif //FuidFire_GridField_HPP


