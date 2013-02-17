//
//  GridField.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/14/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#include "Vector3.h"

#ifdef __APPLE__
#include "glm.hpp"
#elif defined _WIN32 || defined _WIN64
#include <glm/glm.hpp>
#endif
#ifndef FuidFire_GridField_h
#define FuidFire_GridField_h

template <class T> class GridFieldIterator;

template <class T>
class GridField{
private:
    T *_data;
    int _xdim,_ydim, _zdim;
    double *trans,*itrans;
private:
    GridField();
    int size() const;
public:
    //Konstruktor/Destruktor
    GridField(int xdim,int ydim, int zdim);
    GridField(const GridField<T> &g);
    //void operator=(const GridField<T> &g) = delete; Kompilerar ej //Axel c++11?

    void setTransformation(double *m); //4x4 matrix som transformerar cellkoordinater till världskoordinater

    ~GridField();
    

    int xdim() const;
    int ydim() const;
    int zdim() const;

    //Hämta värden
    inline T valueAtIndex(int i) const;
    inline T valueAtIndex(int i,int j,int k) const;
    inline T valueAtWorld(double w_x, double w_y,double w_z) const;
    int cellCount() const;
    

    /**
     Funktioner för att konvertera mellan indexkoordinater, lokalakoordinater och världskoordinater
     */
    
    //Index to index
    inline int indexAt(int i, int j, int k) const;
    inline void indexAt(int index,int &i, int &j, int &k) const;
    
    //Index to...
    inline void indexToWorld(int i,int j,int k, double &w_x, double &w_y,double &w_z) const;
    inline void worldToIndex(int &i,int &j,int &k, double w_x, double w_y,double w_z) const;
    
    //Local to...
    int localToIndex(const double l_x,const double l_y,const double l_z) const;
    inline void localToIndex(int i,int j,int k, double l_x, double l_y,double l_z, double &c_x,double &c_y,double &x_z) const;
    int localToUpperLeftIndex(const double l_x,const double l_y,const double l_z) const; //Används mest internt vid interpolering

    //World to...
    void worldToLocal(const double w_x,const double w_y,const double w_z, double &l_x,double &l_y,double &l_z) const;
    

    
    //Setter
    inline void setValueAtIndex(T val,int i);
    inline void setValueAtIndex(T val,int i,int j,int k);
    
    //Operatorer
    T operator()(int i, int j, int k) const;

    //Iterator
    const GridFieldIterator<T> iterator() const;
    
    friend class GridFieldIterator<T>;
};
/**
    Iterera över gridden.
    Nuvarande index kan hämtas med
*/

template <class T>
class GridFieldIterator {
protected:
    bool _done;
    int _i;
    const GridField<T> *_grid;
protected:
    GridFieldIterator(const GridField<T> *grid):_i(0),_done(false),_grid(grid){  };
public:
    GridFieldIterator(const GridFieldIterator<T> &i):_i(i._i),_done(i._done),_grid(i._grid){  };
    ~GridFieldIterator(){ _grid = NULL; };
    
    void next(){
        if(_i == _grid->cellCount()-1) _done = true;
        if (!done()) _i++;
    };
    
    int index() const{ return _i;};
    bool done() const{return _done;};
    T value() const{ return _grid->valueAtIndex(index());};
    void index(int &i,int &j,int &k) const { _grid->indexAt(index(),i,j,k);};

    friend class GridField<T>;
};


#endif
