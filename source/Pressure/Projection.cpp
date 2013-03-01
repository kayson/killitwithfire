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

            if (Fire::getCellType(_phi->grid->valueAtIndex(i, j, k)) == IGNITED
                || Fire::getCellType(_phi->grid->valueAtIndex(i, j, k)) == BLUECORE) {
                
                if ((Fire::getCellType(_phi->grid->valueAtIndex(i+1, j, k)) == IGNITED
                    || Fire::getCellType(_phi->grid->valueAtIndex(i+1, j, k)) == BLUECORE)
                    && _phi->grid->isValid(i+1, j, k)) {
                    double scale = _dt/(getDensity(i, j, k, RIGHT)*_dx*_dx);
                    A->set_element(row, _phi->grid->indexAt(i+1, j, k), -scale);
                    A->add_to_element(row, row, scale);
                }
                
                if ((Fire::getCellType(_phi->grid->valueAtIndex(i-1, j, k)) == IGNITED
                    || Fire::getCellType(_phi->grid->valueAtIndex(i-1, j, k)) == BLUECORE)
                    && _phi->grid->isValid(i-1, j, k)) {
                    double scale = _dt/(getDensity(i, j, k, LEFT)*_dx*_dx);
                    A->set_element(row, _phi->grid->indexAt(i-1, j, k), -scale);
                    A->add_to_element(row, row, scale);
                }
                
                if ((Fire::getCellType(_phi->grid->valueAtIndex(i, j+1, k)) == IGNITED
                    || Fire::getCellType(_phi->grid->valueAtIndex(i, j+1, k)) == BLUECORE)
                    && _phi->grid->isValid(i, j+1, k)) {
                    double scale = _dt/(getDensity(i, j, k, DOWN)*_dx*_dx);
                    A->set_element(row, _phi->grid->indexAt(i, j+1, k), -scale);
                    A->add_to_element(row, row, scale);
                }
                
                if ((Fire::getCellType(_phi->grid->valueAtIndex(i, j-1, k)) == IGNITED
                    || Fire::getCellType(_phi->grid->valueAtIndex(i, j-1, k)) == BLUECORE)
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

void Projection::fillb(){
    assert(*_phi->grid == *_u);

    //Fill b
    int index = 0;
    for (GridFieldIterator<double> it = _phi->grid->iterator(); !it.done(); it.next()) {
        int i,j,k;
        it.index(i, j, k);

        if (k == 0) {
            
            if (Fire::getCellType(_phi->grid->valueAtIndex(i, j, k)) == BLUECORE
                || Fire::getCellType(_phi->grid->valueAtIndex(i, j, k)) == IGNITED) {
                double scale = _dt/_dx;
                double div = 0;
                
                div -= _u->valueAtFace(i, j, k, RIGHT)*scale/getDensity(i, j, k, RIGHT);
                div = _u->valueAtFace(i, j, k, LEFT)*scale/getDensity(i, j, k, LEFT);
                div -= _u->valueAtFace(i, j, k, DOWN)*scale/getDensity(i, j, k, DOWN);
                div = _u->valueAtFace(i, j, k, UP)*scale/getDensity(i, j, k, UP);
                
                (*b)[index] = div;
            }else{
                (*b)[index] = 0;
            }
            
            index++;
        }

    }
}

void Projection::applyPressure(){
    
    double scaleBluecore = _dt/(_rho_fuel*_dx);
    double scaleIgnited = _dt/(_rho_ignited*_dx);
    int index = 0;
    for (GridFieldIterator<double> it = _phi->grid->iterator(); !it.done(); it.next()) {
        
        int i,j,k;
        it.index(i, j, k);

        if (k == 0) {

            if( Fire::getCellType(_phi->grid->valueAtIndex(i, j, k))  == IGNITED) {
                _u->addValueAtFace(scaleIgnited*(*x)[index], i, j, k, LEFT);
                _u->addValueAtFace(-scaleIgnited*(*x)[index], i, j, k, RIGHT);
                _u->addValueAtFace(scaleIgnited*(*x)[index], i, j, k, DOWN);
                _u->addValueAtFace(-scaleIgnited*(*x)[index], i, j, k, UP);
            }else if ( Fire::getCellType(_phi->grid->valueAtIndex(i, j, k))  == BLUECORE){
                _u->addValueAtFace(scaleBluecore*(*x)[index], i, j, k, LEFT);
                _u->addValueAtFace(-scaleBluecore*(*x)[index], i, j, k, RIGHT);
                _u->addValueAtFace(scaleBluecore*(*x)[index], i, j, k, DOWN);
                _u->addValueAtFace(-scaleBluecore*(*x)[index], i, j, k, UP);
            }
            
            index++;
        }
    
    }
    
}


void Projection::project(double dt,double rho_ignited,double rho_fuel){
    assert(*_phi->grid == *_u);
    
    //Compute size
    _size = _phi->grid->size();
    _dx = _u->dx();
    _dt = dt;
    _rho_ignited = rho_ignited;
    _rho_fuel = rho_fuel;

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
		temp = _phi->grid->valueAtIndex(i,j,k+1);
	if(d == BACKWARD)
		temp = _phi->grid->valueAtIndex(i,j,k-1);
    
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
		temp = Fire::getCellType(_phi->grid->valueAtIndex(i, j, k-1));
	if(d == FORWARD)
		temp = Fire::getCellType(_phi->grid->valueAtIndex(i, j, k+1));

    
	if(Fire::getCellType(_phi->grid->valueAtIndex(i, j, k)) == BLUECORE && temp == BLUECORE)
		return FirePresets::rhof;
	else if(Fire::getCellType(_phi->grid->valueAtIndex(i, j, k)) == BLUECORE && temp == IGNITED)
		return alpha * FirePresets::rhof + ( 1 - alpha ) * FirePresets::rhoh;
	else if(Fire::getCellType(_phi->grid->valueAtIndex(i, j, k)) == IGNITED && temp == BLUECORE)
		return alpha * FirePresets::rhoh + ( 1 - alpha ) * FirePresets::rhof;
	else{// if(getCellType(i,j,k) == IGNITED && temp == IGNITED)
		return FirePresets::rhoh;
    }
}

