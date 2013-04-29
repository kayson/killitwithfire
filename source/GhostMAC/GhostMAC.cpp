//
//  GhostMAC.cpp
//  FuidFire
//
//  Created by Johannes Deligiannis on 3/5/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#include "GhostMAC.h"
#include "firePresets.h"
#include "GhostGrid.hpp"
#include "MACAdvect.h"

void GhostMAC::makeRandom(){
    
    double randMax = 0.0;
    //Fill U
    for (GridFieldIterator<double> iterator = _u->iterator(); !iterator.done(); iterator.next()) {
        int i,j,k;
        double x,y,z;
        iterator.index(i, j, k);
        _u->indexToWorld(i, j, k, x, y, z);
        double v1 = ((double)(rand() % RAND_MAX))/((double)RAND_MAX)*randMax-randMax*0.5;
        double vel = v1;
        _u->setValueAtIndex(vel, iterator.index());
        buffer()->_u->setValueAtIndex(vel, iterator.index());
    }
    
    //Fill V
    for (GridFieldIterator<double> iterator = _v->iterator(); !iterator.done(); iterator.next()) {
        int i,j,k;
        //double x,y,z;
        iterator.index(i, j, k);
        //_v->indexToWorld(i, j, k, x, y, z);
        double v1 =  ((double)(rand() % RAND_MAX))/((double)RAND_MAX)*randMax-randMax*0.5;
        double vel = v1;
        _v->setValueAtIndex(vel, iterator.index());
        buffer()->_v->setValueAtIndex(vel, iterator.index());
    }
    
    //Fill W
    for (GridFieldIterator<double> iterator = _w->iterator(); !iterator.done(); iterator.next()) {
        int i,j,k;
        //double x,y,z;
        iterator.index(i, j, k);
        double v1  = ((double)(rand() % RAND_MAX))/((double)RAND_MAX)*randMax-randMax*0.5;
        double vel = v1;
        _w->setValueAtIndex(vel, iterator.index());
        buffer()->_w->setValueAtIndex(vel, iterator.index());
    }
}

double GhostMAC::valueAtFace(int i,int j,int k,DirectionEnums d, CellType cell) const{
    double x,y,z;
    GridMapping::indexToWorld(i, j, k, x, y, z);
    CellType thisCellType = Fire::getCellType(_levelset->grid->valueAtWorld(x, y, z));
    
    GhostGridField<double> *u = static_cast< GhostGridField<double>* >(_u);
    GhostGridField<double> *v = static_cast< GhostGridField<double>* >(_v);
    GhostGridField<double> *w = static_cast< GhostGridField<double>* >(_w);

	double returnval = 0;

    if (d == RIGHT){
        returnval =  _u->valueAtIndex(i+1, j, k)+u->jump(thisCellType,cell,i+1,j,k);
    }else if (d == LEFT){
        returnval =  _u->valueAtIndex(i, j, k)+u->jump(thisCellType,cell,i,j,k);
    }else if (d == DOWN) {
        returnval =  _v->valueAtIndex(i, j, k)+v->jump(thisCellType,cell,i,j,k);
    }else if (d == UP){
        returnval =  _v->valueAtIndex(i, j+1, k)+v->jump(thisCellType,cell,i,j+1,k);
    }else  if (d == FORWARD){
        returnval =  _w->valueAtIndex(i, j, k)+w->jump(thisCellType,cell,i,j,k);
    }else if(d == BACKWARD){
        returnval =  _w->valueAtIndex(i, j, k+1)+w->jump(thisCellType,cell,i,j,k+1);
    }else{
        //Center?
    }
	return returnval;
    //assert(false);
}

void GhostMAC::initialize(int xdim,int ydim,int zdim, double size){
    
    //Default advektionsalgoritm
    _advect = new MACAdvectRK2<double>();
    
    //Initiera variabler
    _buffer = NULL;
    
    _u = new GhostGridField<double>(xdim+1,ydim,zdim,_levelset,UDIR);
    _v = new GhostGridField<double>(xdim,ydim+1,zdim,_levelset,VDIR);
    _w = new GhostGridField<double>(xdim,ydim,zdim+1,_levelset,WDIR);
    
    _cache = new GridField<Vector3>(xdim,ydim,zdim, new ConstantValueExtrapolation<Vector3>()); //TODO KORREKT EXTRAPOLERING? Används denna ens?
    _cache->setTransformation(glm::dmat4x4(size,0,0,0, 0,size,0,0, 0,0,size,0, 0,0,0,1));
    _hasCache = new GridField<bool>(xdim,ydim,zdim, new ConstantValueExtrapolation<bool>()); //TODO KORREKT EXTRAPOLERING?
    _hasCache->setTransformation(glm::dmat4x4(size,0,0,0, 0,size,0,0, 0,0,size,0, 0,0,0,1));
    _hasCache->setAll(false);
    
    //Skala upp
    setTransformation(glm::scale(size, size, size));
    
    //_u->setValueAtIndex(0.1, 4, 4, 4);
    //_center->setValueAtIndex(-100.0, 4, 4, 4);
}


