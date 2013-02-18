//
//  GridField.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/14/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//




#include "Vector3.h"
#include "GridMapping.h"

#ifdef __APPLE__
#include "glm.hpp"
#elif defined _WIN32 || defined _WIN64
#include <glm/glm.hpp>
#endif
#ifndef FuidFire_GridField_h
#define FuidFire_GridField_h

template <class T> class GridFieldIterator;
template <class T> class GridExtrapolation;

template <class T>
class GridField{
protected:
    T *_data;
    GridExtrapolation<T> *_extrapolation;
public:
    GridMapping mapping;
private:
    GridField();
public:
    //Konstruktor/Destruktor
    GridField(const GridMapping &m);
    GridField(int xdim,int ydim, int zdim);
    GridField(int xdim,int ydim, int zdim, double size);
    GridField(const GridField<T> &g);
    GridField<T>& operator=(const GridField<T> &g);

    ~GridField();
    
    //Dim
    int xdim() const;
    int ydim() const;
    int zdim() const;
    int size() const;

    //Hämta värden
    T valueAtIndex(int i) const;
    T valueAtIndex(int i,int j,int k) const;
    T valueAtWorld(double w_x, double w_y,double w_z) const;
    int cellCount() const;

    //Setter
    void setAll(T val);
    void setValueAtIndex(T val,int i);
    void setValueAtIndex(T val,int i,int j,int k);

    //Operatorer
    T operator()(int i, int j, int k) const;

    //Iterator
    const GridFieldIterator<T> iterator() const;
    
    friend class GridFieldIterator<T>;
};

/**
    Iterera över gridden.
    Nuvarande index kan hämtas
*/

class IteratorCondition{
public:
    virtual bool operator()(int i, int j,int k){ return true; };
};

class IteratorRange : public IteratorCondition{
    int _mini, _minj, _mink,  _maxi, _maxj, _maxk;
public:
    IteratorRange(int mini,int minj,int mink, int maxi,int maxj,int maxk){
        _mini = mini; _minj = minj; _mink = mink; _maxi = maxi; _maxj = maxj; _maxk = maxk;
    }
    
    virtual bool operator()(int i, int j,int k){
        return ( _mini >= i && _maxi >= i && _minj >= j && _maxj >= j && _mink >= k && _maxk >= k);
    }
};

template <class T>
class GridFieldIterator : public GridMappingIterator {
protected:
    const GridField<T> *_grid;
protected:
    GridFieldIterator(const GridField<T> *grid):GridMappingIterator(&grid->mapping),_grid(grid){  };
public:
    GridFieldIterator(const GridFieldIterator<T> &i):GridMappingIterator(&i._grid->mapping),_grid(i._grid){  };
    ~GridFieldIterator(){ _grid = nullptr; };
    T value() const{ return _grid->valueAtIndex(index());};
    friend class GridField<T>;
};


#endif
