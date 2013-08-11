//
//  GridField.cpp
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/14/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//



#ifndef FuidFire_GridField_hpp
#define FuidFire_GridField_hpp
#include "Vector3.h"


#include "GridField.h"
#include "Interpolation.h"
#include "Extrapolation.h"
#include <exception>
#include <stdexcept>
#include <sstream>
#include "Vector3.h"
#include "ConstantValueExtrapolation.h"

#include <iostream>
#ifdef __APPLE__
#elif defined _WIN32 || defined _WIN64
#define round(x) floor((x) >= 0 ? (x) + 0.5 : (x) - 0.5)
#endif
#define nullptr NULL
#include "Extrapolation.h"
#include "ConstantValueExtrapolation.h"

template<class T>
GridField<T>::GridField(const GridMapping &m, Extrapolation<T> *extrapolation):
	GridMapping(m),
	_extrapolation(nullptr),
	_interpolation(nullptr)
{
	_extrapolate = true;
    //Allokera data-array
    _data = new T[cellCount()];
    for (int i = 0; i < cellCount(); i++) _data[i] = T(0);
    
    //Extra/Interpolation
    setInterpolation(Interpolation<T>::defaultInterpolation());
	setExtrapolation(extrapolation);
}


template<class T>
GridField<T>::GridField():
	GridField(10,10,10,
			  new ConstantValueExtrapolation<T>())
{ //TODO KORREKT EXTRAPOLERING?
	_extrapolate = true;
}

/*
template<class T>
GridField<T>::GridField(int xdim,int ydim, int zdim, Extrapolation<T> *extrapolation):GridMapping(xdim,ydim,zdim),_extrapolation(nullptr),_interpolation(nullptr),_extrapolate(true){    
    //Allokera data-array
    _data = new T[cellCount()];
    for (int i = 0; i < cellCount(); i++) _data[i] = T();
    
    //Extra/Interpolation
    setInterpolation(Interpolation<T>::defaultInterpolation());
	setExtrapolation(extrapolation);
}*/

template<class T>
GridField<T>::GridField(int xdim,int ydim, int zdim, double size, Extrapolation<T> *extrapolation):GridMapping(xdim,ydim,zdim,size),_extrapolation(nullptr),_interpolation(nullptr),_extrapolate(true){
    
    //Allokera data-array
    _data = new T[cellCount()];
    for (int i = 0; i < cellCount(); i++) _data[i] = T();

    //Extra/Interpolation
    setInterpolation(Interpolation<T>::defaultInterpolation());
	setExtrapolation(extrapolation);
}

template<class T>
GridField<T>::GridField(const GridField<T> &g):GridMapping(g),_extrapolation(nullptr),_interpolation(nullptr),_extrapolate(true){
    int count = g.cellCount();
    _data = new T[count];

    //Iterera över g
    for (GridFieldIterator<T> iter = g.iterator(); !iter.done(); iter.next()) {
        setValueAtIndex(iter.value(), iter.index());
    }
    
    //Interpolation
    //Extra/Interpolation
    setInterpolation(g._interpolation);
	setExtrapolation(g._extrapolation);
}

template<class T>
GridField<T>& GridField<T>::operator=(const GridField<T> &g){
    if (this != &g) {
        
        //Allokera data-array
        if(g.cellCount() != cellCount()){
            delete _data;
            _data = new T[g.cellCount()];
        }
        std::copy(g._data,g._data+g.cellCount(), _data);

        setInterpolation(g._interpolation);
        setExtrapolation(g._extrapolation);
    }
    
    return *this;
}


template<class T>
GridField<T>::~GridField(){
    delete [] _data;
    if(_interpolation != nullptr) delete _interpolation;
	if(_extrapolation != nullptr) delete _extrapolation;
}

template<class T>
void GridField<T>::setInterpolation(const Interpolation<T> *i){
    
    if (_interpolation != nullptr) {
        delete _interpolation;
    }
    _interpolation = i != nullptr ?  i->clone() : nullptr;
}

template<class T>
void GridField<T>::setExtrapolation(const Extrapolation<T> *e){
    if (_extrapolation != nullptr) {
        delete _extrapolation;
    }
    _extrapolation = e != nullptr ? e->clone() : nullptr;
}

template<class T>
void GridField<T>::disableExtrapolation(){
    _extrapolate = false;
}
template<class T>
void GridField<T>::enableExtrapolation(){
    _extrapolate = true;
}

template<class T>
bool GridField<T>::isUndefined(int i) const{
    return i < 0 || i >= cellCount();
}

template<class T>
bool GridField<T>::isUndefined(int i,int j,int k) const{
    return i < 0 || i >= xdim() || j < 0 || j >= ydim() || k < 0 || k >= zdim();
}

template<class T>
T GridField<T>::valueAtIndex(int i) const{
	if(isUndefined(i)){
        std::stringstream ss;
        ss << "Försökte komma åt ett odefinierat index (" << i << ") i GridField::valueAtIndex(int i)";
        throw std::out_of_range (ss.str());
    }
    return _data[i];
}

template<class T>
T GridField<T>::valueAtIndex(int i,int j,int k) const{
    if (_extrapolate && isUndefined(i, j, k)){
        if(_extrapolation == nullptr){
            std::stringstream ss;
            ss << "Försökte komma åt ett odefinierat index (" << i;
            ss << "), (" << j << "), (" << k;
            ss << ") i GridField::valueAtIndex(int i,int j,int k).";
            ss << "En möjlig anledning kan vara att GridField._extrapolation är NULL";
            throw std::out_of_range(ss.str());
        }
            
        return _extrapolation->extrapolate(*this,i,j,k);
    }else{
        return valueAtIndex(indexAt(i, j, k));
    }
}

template<class T>
T GridField<T>::valueAtWorld(double w_x, double w_y,double w_z) const{
    
    int i,j,k;
    double w_x0,w_y0,w_z0;
    double w_x1,w_y1,w_z1;
    double x,y,z;

    //Konvertera världskoordinater till cellkoordinater
    worldToUpperLeftIndex(w_x, w_y, w_z, i, j, k);
    indexToWorld(i, j, k, w_x0, w_y0, w_z0);
    indexToWorld(i+1, j+1, k+1, w_x1, w_y1, w_z1);

    x = (w_x-w_x0)/(w_x1-w_x0);
    y = (w_y-w_y0)/(w_y1-w_y0);
    z = (w_z-w_z0)/(w_z1-w_z0);
    //assert(i < 100);
    //FIXA!!! -->    
    //Interpolera...
    
    /*
    
    CatmullRomInterpolation<T> interpolation = CatmullRomInterpolation<T>();
    T ty[4];
    for (int slice = 0; slice < 4; slice++) {
        T t1 = interpolation(x,valueAtIndex(i-1,j-1,k),valueAtIndex(i,j-1,k),valueAtIndex(i+1,j-1,k),valueAtIndex(i+2,j-1,k));
        T t2 = interpolation(x,valueAtIndex(i-1,j,k),valueAtIndex(i,j,k),valueAtIndex(i+1,j,k),valueAtIndex(i+2,j,k));
        T t3 = interpolation(x,valueAtIndex(i-1,j+1,k),valueAtIndex(i,j+1,k),valueAtIndex(i+1,j+1,k),valueAtIndex(i+2,j+1,k));
        T t4 = interpolation(x,valueAtIndex(i-1,j+2,k),valueAtIndex(i,j+2,k),valueAtIndex(i+1,j+2,k),valueAtIndex(i+2,j+2,k));
        
        ty[slice] = interpolation(y,t1,t2,t3,t4);
    }
     
    */
    
    
    return _interpolation->interpolate(*this,i,j,k,x,y,z);
}

template<class T> 
T GridField<T>::getMax() const
{
	T max;
	GridFieldIterator<T> it = iterator();
	if(!it.done())
		max = valueAtIndex(it.index());
	else
	{
		std::cout << "GridField<T>::getMax() failed!" << std::endl;
		throw;
	}
	
	for(; !it.done(); it.next())
	{
		T v = valueAtIndex(it.index());
		if(v > max)
			max = v;
	}

	return max;
}

template<class T>
int GridField<T>::cellCount() const{
    return size();
}

template<class T>
void GridField<T>::setValueAtIndex(T val,int i){
    _data[i] = val;
}
template<class T>
void GridField<T>::addValueAtIndex(T val,int i){
    _data[i] += val;
}
template<class T>
void GridField<T>::setAll(T val){
    for (int i = 0; i < cellCount(); i++) _data[i] = val;
}

template<class T>
void GridField<T>::setValueAtIndex(T val,int i,int j,int k){
    int index = indexAt(i, j, k);
    /*if (typeid(T) == typeid(bool) && i == 2 && j == 44 && k == 0) {
        std::cout << cellCount() << std::endl;
    }*/
    _data[index] = val;
}

template<class T>
void GridField<T>::addValueAtIndex(T val,int i,int j,int k){
    int index = indexAt(i, j, k);
    _data[index] += val;
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


