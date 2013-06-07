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
private:
	 GridMapping();
protected:
    glm::dmat4x4 _trans;  //Local coordinates to worldcoordinates
    glm::dmat4x4 _itrans; //World coordinates to localcoordinates
    double *_fast_trans;
    double *_fast_itrans;
    int _xdim,_ydim,_zdim;
    int _size;
    
protected:
    //Local dim
    double local_dx() const;
    double local_dy() const;
    double local_dz() const;
public:
    //Konstruktor
    //GridMapping(unsigned int xdim,unsigned int ydim,unsigned int zdim);
    GridMapping(unsigned int xdim,unsigned int ydim,unsigned int zdim, double size);
    GridMapping(unsigned int dim,glm::dmat4x4 t);
    GridMapping(unsigned int xdim,unsigned int ydim,unsigned int zdim,glm::dmat4x4 t);
    GridMapping(const GridMapping &);
    ~GridMapping();
    GridMapping & operator= (const GridMapping & other);
    bool operator== (const GridMapping & other);


    //Transformation
    void setTransformation(glm::dmat4x4 t);
    void multTransformation(glm::dmat4x4 t);
    glm::dmat4x4 getTrans() const;
    
    //Dim
    int xdim() const;
    int ydim() const;
    int zdim() const;
    int size() const;
    double dx() const;
    double dy() const;
    double dz() const;

    //Index to index
    int indexAt(int i, int j, int k) const;
    void indexAt(int index,int &i, int &j, int &k) const;
    bool isValid(int i, int j, int k) const;

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

	//valid
	static bool localIsValid(double l_x, double l_y,double l_z);
	bool worldIsValid(const double w_x,const double w_y,const double w_z) const;
    
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
