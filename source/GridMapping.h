//
//  GridMapping.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/17/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FuidFire__GridMapping__
#define __FuidFire__GridMapping__

#ifdef __APPLE__
#include "glm.hpp"
#elif defined _WIN32 || defined _WIN64
#include <glm/glm.hpp>
#endif

#include "Vector3.h"

class GridMappingIterator;

class GridMapping {
protected:
    glm::mat4x4 _trans;  //Local coordinates to worldcoordinates
    glm::mat4x4 _itrans; //World coordinates to localcoordinates
    int _xdim,_ydim,_zdim;
    int _size;
public:
    //Konstruktor
    GridMapping();
    GridMapping(int xdim,int ydim,int zdim);
    GridMapping(int xdim,int ydim,int zdim, double size);
    GridMapping(int dim,glm::mat4x4 t);
    GridMapping(int xdim,int ydim,int zdim,glm::mat4x4 t);
    GridMapping(const GridMapping &);
    ~GridMapping();
    GridMapping & operator= (const GridMapping & other);
    
    //Transformation
    void setTransformation(double *t);
    void setTransformation(glm::mat4x4 t);
    
    //Dim
    int xdim() const;
    int ydim() const;
    int zdim() const;
    int size() const;

    //Index to index
    int indexAt(int i, int j, int k) const;
    void indexAt(int index,int &i, int &j, int &k) const;
    
    //Index to...
    void indexToWorld(int i,int j,int k, double &w_x, double &w_y,double &w_z) const;
    void indexToLocal(int i,int j,int k, double &l_x, double &l_y,double &l_z) const;
    
    //Local to...
    void localToIndex(double l_x, double l_y,double l_z,int &i,int &j,int &k) const;
    void localToWorld(double l_x, double l_y,double l_z, double &w_x,double &w_y,double &w_z) const;
    void localToUpperLeftIndex(const double l_x,const double l_y,const double l_z, int &i,int &j,int &k) const;
    
    //World to...
    void worldToLocal(const double w_x,const double w_y,const double w_z, double &l_x,double &l_y,double &l_z) const;
    void worldToIndex(int &i,int &j,int &k, double w_x, double w_y,double w_z) const;
    void worldToUpperLeftIndex(const double w_x,const double w_y,const double w_z, int &i,int &j,int &k) const;
    
    GridMappingIterator iterator() const;
};

class GridMappingIterator {
protected:
    bool _done;
    int _i;
    const GridMapping *_mapping;
protected:
    GridMappingIterator(const GridMapping *mapping):_i(0),_done(false),_mapping(mapping){  };
public:
    GridMappingIterator(const GridMappingIterator &i):_i(i._i),_done(i._done),_mapping(i._mapping){  };
    ~GridMappingIterator(){ _mapping = nullptr; };
    void step(){
        _i++;
        if(_i >= _mapping->xdim()*_mapping->ydim()*_mapping->zdim()) _done = true;
    };
    
    void next(){
        if (done()) return;
        step();
    };
    
    int index() const{ return _i;};
    bool done() const{return _done;};
    void index(int &i,int &j,int &k) const { _mapping->indexAt(index(),i,j,k);};
    
    friend class GridMapping;
};



#endif /* defined(__FuidFire__GridMapping__) */
