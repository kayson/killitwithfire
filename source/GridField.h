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

template <class T>
class GridFieldIterator;

template <class T>
class GridField{
private:
    T *_data;
    const int _xdim,_ydim, _zdim;
    double *trans,*itrans;
private:
    GridField();
public:
    GridField(int xdim,int ydim, int zdim);

    ~GridField();

    void setTransformation(double *m); //4x4 matrix som transformerar cellkoordinater till världskoordinater
    
    //Konvertera
    inline void cellToWorld(int i,int j,int k, double &w_x, double &w_y,double &w_z) const;
    inline void worldToCell(int &i,int &j,int &k, double &w_x, double &w_y,double &w_z) const;
    
    //Getter
    
    int xdim() const;
    int ydim() const;
    int zdim() const;

    inline T valueAtIndex(int i) const;
    inline T valueAtIndex(int i,int j,int k) const;
    inline T valueAtWorld(double w_x, double w_y,double w_z) const;
    int cellCount() const;
    
    inline int indexAt(int i, int j, int k) const;
    inline void indexAt(int index,int &i, int &j, int &k) const;
    void worldToLocal(const double w_x,const double w_y,const double w_z, double &l_x,double &l_y,double &l_z) const;
    int localToIndex(const double l_x,const double l_y,const double l_z) const;
    int localToUpperLeftIndex(const double l_x,const double l_y,const double l_z) const;
    inline void localToCellCoordinate(int i,int j,int k, double l_x, double l_y,double l_z, double &c_x,double &c_y,double &x_z) const;

    
    //Setter
    inline void setValueAtIndex(T val,int i);
    inline void setValueAtIndex(T val,int i,int j,int k);
    
    //Operatorer
    T operator()(int i, int j, int k);
    
    //Iterator
    GridFieldIterator<T> iterator();
    
    friend class GridFieldIterator<T>;
};
/**
    Iterera över gridden.
    Nuvarande index kan hämtas med
*/
template <class T>
class GridFieldIterator {
private:
    bool _done;
    int _i;
    GridField<T> *_grid;
private:
    GridFieldIterator(GridField<T> *grid):_i(0),_done(false){ _grid = grid; };
    int index(){ return _i;};
public:
    ~GridFieldIterator(){ _grid = NULL; };
public:
    void next(){ _i++; if(_i == _grid->cellCount()) _done = true;};
    bool done(){return _done;};
    T value(){ return _grid->valueAtIndex(index());};
    void setValue(double val){ _grid->setValueAtIndex(val,index());};
    void index(int &i,int &j,int &k){ _grid->indexAt(index(),i,j,k);};

    friend class GridField<T>;
};

#endif
