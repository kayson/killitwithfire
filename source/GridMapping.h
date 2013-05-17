//
//  GridMapping.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/17/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FuidFire__GridMapping__
#define __FuidFire__GridMapping__

#if defined __APPLE__ || __unix__
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
    float *_fast_trans;
    float *_fast_itrans;
    int _xdim,_ydim,_zdim;
    int _size;
    
protected:
    //Local dim
    float local_dx() const;
    float local_dy() const;
    float local_dz() const;
public:
    //Konstruktor
    GridMapping();
    GridMapping(unsigned int xdim,unsigned int ydim,unsigned int zdim);
    GridMapping(unsigned int xdim,unsigned int ydim,unsigned int zdim, float size);
    GridMapping(unsigned int dim,glm::mat4x4 t);
    GridMapping(unsigned int xdim,unsigned int ydim,unsigned int zdim,glm::mat4x4 t);
    GridMapping(const GridMapping &);
    ~GridMapping();
    GridMapping & operator= (const GridMapping & other);
    bool operator== (const GridMapping & other);


    //Transformation
    void setTransformation(glm::mat4x4 t);
    void multTransformation(glm::mat4x4 t);
    glm::mat4x4 getTrans() const;
    
    //Dim
    int xdim() const;
    int ydim() const;
    int zdim() const;
    int size() const;
    float dx() const;
    float dy() const;
    float dz() const;

    //Index to index
    int indexAt(int i, int j, int k) const;
    void indexAt(int index,int &i, int &j, int &k) const;
    bool isValid(int i, int j, int k) const;

    //Index to...
    void indexToWorld(int i,int j,int k, float &w_x, float &w_y,float &w_z) const;
    void indexToLocal(int i,int j,int k, float &l_x, float &l_y,float &l_z) const;
    
    //Local to...
    void localToIndex(float l_x, float l_y,float l_z,int &i,int &j,int &k) const;
    void localToWorld(float l_x, float l_y,float l_z, float &w_x,float &w_y,float &w_z) const;
    void localToUpperLeftIndex(const float l_x,const float l_y,const float l_z, int &i,int &j,int &k) const;
    
    //World to...
    void worldToLocal(const float w_x,const float w_y,const float w_z, float &l_x,float &l_y,float &l_z) const;
    void worldToIndex(int &i,int &j,int &k, float w_x, float w_y,float w_z) const;
    void worldToUpperLeftIndex(const float w_x,const float w_y,const float w_z, int &i,int &j,int &k) const;
    
    GridMappingIterator iterator() const;

};

class GridMappingIterator {
protected:
    bool _done;
    int _i;
    const GridMapping *_mapping;
protected:
    GridMappingIterator(const GridMapping *mapping):_done(false),_i(0),_mapping(mapping){  };
public:
    GridMappingIterator(const GridMappingIterator &i):_done(i._done),_i(i._i),_mapping(i._mapping){  };
    ~GridMappingIterator(){ _mapping = NULL; };
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
