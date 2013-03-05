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
            
            if ((Fire::getCellType(_phi->grid->valueAtIndex(i, j, k)) == BURNT
                 || Fire::getCellType(_phi->grid->valueAtIndex(i, j, k)) == FUEL)
                && !(i == 0 || j == 0 || i == (_phi->grid->xdim()-1) || j == (_phi->grid->ydim()-1))) {
                //A.k.a Solid...
                if (i+1 == 0 || j == 0 || i+1 == (_phi->grid->xdim()-1) || j == (_phi->grid->ydim()-1)) {
                    
                }else if (_phi->grid->isValid(i+1, j, k)) {
                    double scale = _dt/(_dx*_dx);
                    A->set_element(row, _phi->grid->indexAt(i+1, j, k), -scale/getDensity(i, j, k, RIGHT));
                    
                    
                    if (Fire::getCellType(_phi->grid->valueAtIndex(i+1, j, k)) == BURNT) {
                        A->add_to_element(row, row, scale/FirePresets::rhof);
                    }else{
                        A->add_to_element(row, row, scale/FirePresets::rhob);
                    }
                    
                }
                
                if (i-1 == 0 || j == 0 || i-1 == (_phi->grid->xdim()-1) || j == (_phi->grid->ydim()-1)) {
                    //A.k.a Solid...
                }else if (_phi->grid->isValid(i-1, j, k)) {
                    double scale = _dt/(_dx*_dx);
                    A->set_element(row, _phi->grid->indexAt(i-1, j, k), -scale/getDensity(i, j, k, LEFT));
                    
                    if (Fire::getCellType(_phi->grid->valueAtIndex(i-1, j, k)) == BURNT) {
                        A->add_to_element(row, row, scale/FirePresets::rhof);
                    }else{
                        A->add_to_element(row, row, scale/FirePresets::rhob);
                    }
                    
                }
                
                if (i == 0 || j+1 == 0 || i == (_phi->grid->xdim()-1) || j+1 == (_phi->grid->ydim()-1)) {
                    //A.k.a Solid...
                }else if(_phi->grid->isValid(i, j+1, k)){
                    double scale = _dt/(_dx*_dx);
                    A->set_element(row, _phi->grid->indexAt(i, j+1, k), -scale/getDensity(i, j, k, UP));
                    
                    if (Fire::getCellType(_phi->grid->valueAtIndex(i, j+1, k)) == BURNT) {
                        A->add_to_element(row, row, scale/FirePresets::rhof);
                    }else{
                        A->add_to_element(row, row, scale/FirePresets::rhob);
                    }
                }
                
                if (i == 0 || j-1 == 0 || i == (_phi->grid->xdim()-1) || j-1 == (_phi->grid->ydim()-1)) {
                    //A.k.a Solid...
                }else if(_phi->grid->isValid(i, j-1, k)) {
                    double scale = _dt/(_dx*_dx);
                    A->set_element(row, _phi->grid->indexAt(i, j-1, k), -scale/getDensity(i, j, k, DOWN));
                    
                    if (Fire::getCellType(_phi->grid->valueAtIndex(i, j-1, k)) == BURNT) {
                        A->add_to_element(row, row, scale/getDensity(i, j, k, DOWN));
                    }else{
                        A->add_to_element(row, row, scale/FirePresets::rhob);
                    }
                    
                }
            }
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
    
    //Fill b
    int index = 0;
    for (GridFieldIterator<double> it = _phi->grid->iterator(); !it.done(); it.next()) {
        int i,j,k;
        it.index(i, j, k);
        
        if (k == 0) {
            
            if (Fire::getCellType(_phi->grid->valueAtIndex(i, j, k)) == FUEL
                || Fire::getCellType(_phi->grid->valueAtIndex(i, j, k)) == BURNT) {
                double d = 0;
                double scale = _dt/_dx;
                CellType currType = Fire::getCellType(_phi->grid->valueAtIndex(i, j, k));
                
                
                d -= _u->valueAtFace(i, j, k, RIGHT)*scale/getDensity(i, j, k, RIGHT);
                d += _u->valueAtFace(i, j, k, LEFT)*scale/getDensity(i, j, k, LEFT);
                d -= _u->valueAtFace(i, j, k, DOWN)*scale/getDensity(i, j, k, DOWN);
                d += _u->valueAtFace(i, j, k, UP)*scale/getDensity(i, j, k, UP);
                
                (*b)[index] = d;
            }else{
                (*b)[index] = 0;
            }
            
            index++;
        }
        
    }
}

void Projection::applyPressure(){
    
    int index = 0;
    for (GridFieldIterator<double> it = _phi->grid->iterator(); !it.done(); it.next()) {
        
        int i,j,k;
        it.index(i, j, k);
        
        if (k == 0) {
            _u->addValueAtFace(getDensity(i, j, k, LEFT)*(*x)[index], i, j, k, LEFT);
            _u->addValueAtFace(-getDensity(i, j, k, RIGHT)*(*x)[index], i, j, k, RIGHT);
            _u->addValueAtFace(getDensity(i, j, k, DOWN)*(*x)[index], i, j, k, DOWN);
            _u->addValueAtFace(-getDensity(i, j, k, UP)*(*x)[index], i, j, k, UP);
            index++;
        }
    }
    
    
    /*index = 0;
     for (GridFieldIterator<double> it = _phi->grid->iterator(); !it.done(); it.next()) {
     
     int i,j,k;
     it.index(i, j, k);
     
     if (k == 0) {
     //Draw pressure
     double x,y,z;
     _phi->grid->indexToWorld(i, j, k, x, y, z);
     double p = (*b)[index]/max_pressure;
     double dx = _phi->grid->dx();
     glColor3d(p == 0 ? 0.0 : 0, p < 0 ? fabs(p) : 0, p > 0 ? p : 0);
     glBegin(GL_QUADS);
     glVertex3d(x-0.5*dx, y-0.5*dx, 0);
     glVertex3d(x+0.5*dx, y-0.5*dx, 0);
     glVertex3d(x+0.5*dx, y+0.5*dx, 0);
     glVertex3d(x-0.5*dx, y+0.5*dx, 0);
     glVertex3d(x-0.5*dx, y-0.5*dx, 0);
     glEnd();
     
     index++;
     }
     }*/
    
    
    
    
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
	// sid. 104 (Bridson)
	double otherCell = 0;
    double thisCell = _phi->grid->valueAtIndex(i,j,k);
    
	if(d == RIGHT)
		otherCell = _phi->grid->valueAtIndex(i+1,j,k);
	if(d == LEFT)
		otherCell = _phi->grid->valueAtIndex(i-1,j,k);
	if(d == UP)
		otherCell = _phi->grid->valueAtIndex(i,j+1,k);
	if(d == DOWN)
		otherCell = _phi->grid->valueAtIndex(i,j-1,k);
	if(d == FORWARD)
		otherCell = _phi->grid->valueAtIndex(i,j,k-1);
	if(d == BACKWARD)
		otherCell = _phi->grid->valueAtIndex(i,j,k+1);
    
	if(Fire::getCellType(thisCell) == FUEL && Fire::getCellType(otherCell) == FUEL){
		return 1;
	}else if(Fire::getCellType(thisCell) == FUEL && Fire::getCellType(otherCell) == BURNT){
        double val = (thisCell /(thisCell - otherCell));
        if (std::isnan(val)) {
            throw;
        }
		return val;
    }else if(Fire::getCellType(thisCell) == BURNT && Fire::getCellType(otherCell) == FUEL){
		double val =  1.0 - (thisCell /(thisCell - otherCell));
        if(val > 10*10*10) val = 10*10*10;
        return val;
	}else{// if(phi.grid->valueAtIndex(i,j,k) > 0 && temp > 0)
		return 0;
    }
}

double Projection::getDensity(int i, int j, int k, DirectionEnums d) const{
	// sid. 104 (Bridson)
    double alfa = getAlpha(i, j,k, d);
    return FirePresets::rhof*alfa+(1.0-alfa)*FirePresets::rhob;
}

