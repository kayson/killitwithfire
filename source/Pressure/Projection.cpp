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

            if (Fire::getCellType(_phi->grid->valueAtIndex(i, j, k)) == BURNT
                || Fire::getCellType(_phi->grid->valueAtIndex(i, j, k)) == FUEL) {
                
                if ((Fire::getCellType(_phi->grid->valueAtIndex(i+1, j, k)) == BURNT
                    || Fire::getCellType(_phi->grid->valueAtIndex(i+1, j, k)) == FUEL)
                    && _phi->grid->isValid(i+1, j, k)) {
                    double scale = _dt/(getDensity(i, j, k, RIGHT)*_dx*_dx);
                    A->set_element(row, _phi->grid->indexAt(i+1, j, k), -scale);
                    A->add_to_element(row, row, scale);
                }else{
                    double scale = _dt/(FirePresets::rhof*_dx*_dx);
                    A->add_to_element(row, row, scale);
                }
                
                if ((Fire::getCellType(_phi->grid->valueAtIndex(i-1, j, k)) == BURNT
                    || Fire::getCellType(_phi->grid->valueAtIndex(i-1, j, k)) == FUEL)
                    && _phi->grid->isValid(i-1, j, k)) {
                    double scale = _dt/(getDensity(i, j, k, LEFT)*_dx*_dx);
                    A->set_element(row, _phi->grid->indexAt(i-1, j, k), -scale);
                    A->add_to_element(row, row, scale);
                }
                
                if ((Fire::getCellType(_phi->grid->valueAtIndex(i, j+1, k)) == BURNT
                    || Fire::getCellType(_phi->grid->valueAtIndex(i, j+1, k)) == FUEL)
                    && _phi->grid->isValid(i, j+1, k)) {
                    double scale = _dt/(getDensity(i, j, k, DOWN)*_dx*_dx);
                    A->set_element(row, _phi->grid->indexAt(i, j+1, k), -scale);
                    A->add_to_element(row, row, scale);
                }
                
                if ((Fire::getCellType(_phi->grid->valueAtIndex(i, j-1, k)) == BURNT
                    || Fire::getCellType(_phi->grid->valueAtIndex(i, j-1, k)) == FUEL)
                    && _phi->grid->isValid(i, j-1, k)) {
                    double scale = _dt/(getDensity(i, j, k, UP)*_dx*_dx);
                    A->set_element(row, _phi->grid->indexAt(i, j-1, k), -scale);
                    A->add_to_element(row, row, scale);
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
                
                d -= div(i, j, k, RIGHT, currType)*scale/getDensity(i, j, k, RIGHT);
                d += div(i, j, k, LEFT,currType)*scale/getDensity(i, j, k, LEFT);
                d -= div(i, j, k, DOWN,currType)*scale/getDensity(i, j, k, DOWN);
                d += div(i, j, k, UP,currType)*scale/getDensity(i, j, k, UP);
                
                (*b)[index] = d;
            }else{
                (*b)[index] = 0;
            }
            
            index++;
        }

    }
}

void Projection::applyPressure(){
    
    double scaleFUEL = _dt/(FirePresets::rhof*_dx);
    double scaleBURNT = _dt/(FirePresets::rhob*_dx);
    int index = 0;
    double max_pressure = 0;
    for (GridFieldIterator<double> it = _phi->grid->iterator(); !it.done(); it.next()) {
        
        int i,j,k;
        it.index(i, j, k);

        if (k == 0) {

            if( Fire::getCellType(_phi->grid->valueAtIndex(i, j, k))  == BURNT) {
                _u->addValueAtFace(scaleBURNT*(*x)[index], i, j, k, LEFT);
                _u->addValueAtFace(-scaleBURNT*(*x)[index], i, j, k, RIGHT);
                _u->addValueAtFace(scaleBURNT*(*x)[index], i, j, k, DOWN);
                _u->addValueAtFace(-scaleBURNT*(*x)[index], i, j, k, UP);
            }else if ( Fire::getCellType(_phi->grid->valueAtIndex(i, j, k))  == FUEL){
                _u->addValueAtFace(scaleFUEL*(*x)[index], i, j, k, LEFT);
                _u->addValueAtFace(-scaleFUEL*(*x)[index], i, j, k, RIGHT);
                _u->addValueAtFace(scaleFUEL*(*x)[index], i, j, k, DOWN);
                _u->addValueAtFace(-scaleFUEL*(*x)[index], i, j, k, UP);
            }
            
            if (fabs((*b)[index]) > max_pressure) {
                max_pressure  = fabs((*b)[index]);
            }
            
            
            index++;
        }
    }
    

    index = 0;
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
    }
    

    
    
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
    solver.solve(*A, *b, *x, residual, iterations); //Gör magi...
    applyPressure();
}

double Projection::getAlpha(const int i, const int j, const int k, DirectionEnums d)
{
	// sid. 104 (Bridson)
	double temp = 0;
    
	if(d == RIGHT)
		temp = _phi->grid->valueAtIndex(i+1,j,k);
	if(d == LEFT)
		temp = _phi->grid->valueAtIndex(i-1,j,k);
	if(d == UP)
		temp = _phi->grid->valueAtIndex(i,j+1,k);
	if(d == DOWN)
		temp = _phi->grid->valueAtIndex(i,j-1,k);
	if(d == FORWARD)
		temp = _phi->grid->valueAtIndex(i,j,k-1);
	if(d == BACKWARD)
		temp = _phi->grid->valueAtIndex(i,j,k+1);
    
	if(_phi->grid->valueAtIndex(i,j,k) <= 0 && temp <= 0)
		return 1;
	else if(_phi->grid->valueAtIndex(i,j,k) <= 0 && temp > 0)
		return (_phi->grid->valueAtIndex(i,j,k) /
                (_phi->grid->valueAtIndex(i,j,k) - temp));
	else if(_phi->grid->valueAtIndex(i,j,k) > 0 && temp <= 0)
		return 1.0 - (_phi->grid->valueAtIndex(i,j,k) /
                    (_phi->grid->valueAtIndex(i,j,k) - temp));
	else{// if(phi.grid->valueAtIndex(i,j,k) > 0 && temp > 0)
		return 0;
    }
    
}

double Projection::getDensity(const int i, const int j, const int k, DirectionEnums d){
	// sid. 104 (Bridson)
    //return densityAt(i, j, k, d);
	double alpha = getAlpha(i,j,k,d);
	CellType temp;
    
	if(d == LEFT)
		temp = Fire::getCellType(_phi->grid->valueAtIndex(i-1, j, k));
	if(d == RIGHT)
		temp = Fire::getCellType(_phi->grid->valueAtIndex(i+1, j, k));
	if(d == DOWN)
		temp = Fire::getCellType(_phi->grid->valueAtIndex(i, j-1, k));
	if(d == UP)
		temp = Fire::getCellType(_phi->grid->valueAtIndex(i, j+1, k));
	if(d == BACKWARD)
		temp = Fire::getCellType(_phi->grid->valueAtIndex(i, j, k+1));
	if(d == FORWARD)
		temp = Fire::getCellType(_phi->grid->valueAtIndex(i, j, k-1));

    
	if(Fire::getCellType(_phi->grid->valueAtIndex(i, j, k)) == FUEL && temp == FUEL)
		return FirePresets::rhof;
	else if(Fire::getCellType(_phi->grid->valueAtIndex(i, j, k)) == FUEL && temp == BURNT)
		return alpha * FirePresets::rhof + ( 1 - alpha ) * FirePresets::rhob;
	else if(Fire::getCellType(_phi->grid->valueAtIndex(i, j, k)) == BURNT && temp == FUEL)
		return alpha * FirePresets::rhob + ( 1 - alpha ) * FirePresets::rhof;
	else{// if(getCellType(i,j,k) == BURNT && temp == BURNT)
		return FirePresets::rhob;
    }
}

