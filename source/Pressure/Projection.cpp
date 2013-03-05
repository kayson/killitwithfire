//
//  Projection2.cpp
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/22/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//


#include "Projection.h"
#include <iomanip>
#include "glfw.h"
#include "pcgsolver/pcg_solver.h"
#include "fire.h"
#include "../presets/firePresets.h"
void Projection::resize(){

    if(A == nullptr ){
        A = new SparseMatrix<double>(_size,5);
    }
    
    A->zero();
    
    if (b == nullptr){
        b = new std::vector<double>(_size);
    }
    std::fill(b->begin(), b->end(), 0);
    
    if (x == nullptr){
        x = new std::vector<double>(_size);
    }
    
}

void Projection::fillA(){
    assert(*_phi->grid == *_u);
    int row = 0;

    for (GridMappingIterator it = _u->iterator(); !it.done(); it.next()) {
            int i,j,k;
            it.index(i, j, k);
        if (k == 0) {
            
            row++;
        }
    }
}


double Projection::div(int i ,int j ,int k,DirectionEnums d, CellType centerCellType){
    static double DV = (FirePresets::rhof/FirePresets::rhob-1.0)*FirePresets::S;

    double x,y,z;
    _u->halfIndexToWorld(i, j, k, d, x, y, z);
    if (Fire::getCellType(_phi->grid->valueAtWorld(x,y,z)) == BURNT && centerCellType == BURNT) {
        return _u->valueAtFace(i, j, k, d);
    }else if(Fire::getCellType(_phi->grid->valueAtWorld(x,y,z)) == FUEL && centerCellType == FUEL){
        return _u->valueAtFace(i, j, k, d);
    }else if (Fire::getCellType(_phi->grid->valueAtWorld(x,y,z)) == BURNT && centerCellType == FUEL){
        //På flame front
        Vector3 n = _phi->getNormal(x, y, z);
        n.mult(DV);
        
        if (d == LEFT || d == RIGHT) {
            return _u->valueAtFace(i, j, k, d)+n.x;
        }else if (d == UP || d == DOWN){
            return _u->valueAtFace(i, j, k, d)+n.y;
        }else if(d == FORWARD || d == BACKWARD){
            
        }
        
    }else{// if (Fire::getCellType(_phi->grid->valueAtWorld(x,y,z)) == BURNT && centerCellType == FUEL){
        //På flame front
        Vector3 n = _phi->getNormal(x, y, z);
        n.mult(DV);
        
        if (d == LEFT || d == RIGHT) {
            return _u->valueAtFace(i, j, k, d)-n.x;
        }else if (d == UP || d == DOWN){
            return _u->valueAtFace(i, j, k, d)-n.y;
        }else if(d == FORWARD || d == BACKWARD){
            
        }
    }
}

void Projection::fillb(){
    assert(*_phi->grid == *_u);

   }

void Projection::applyPressure(){
    
    
    
}


void Projection::project(double dt){
    assert(*_phi->grid == *_u);
    
    //Compute size
    _size = _phi->grid->size();
    _dx = _u->dx();
    _dt = dt;

    resize(); //Sätt storlekar på arrayer/matriser
    fillA(); //Fyll A-matrisen
    fillb(); //Fyll b-matrisen

    double residual;
    int iterations;
    PCGSolver<double> solver;
    if(solver.solve(*A, *b, *x, residual, iterations)){
        applyPressure();
    }
}

double Projection::getAlpha(int i, int j, int k, DirectionEnums d) const{
	
    return 0;
}

double Projection::getDensity(int i, int j, int k, DirectionEnums d) const{
	return 0;
}

