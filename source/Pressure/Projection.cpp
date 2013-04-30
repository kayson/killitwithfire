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
#include <cmath>

void Projection::resize(){
    
    if(A == NULL ){
        A = new SparseMatrix<double>(_size,5);
    }
    
    if (b == NULL){
        b = new std::vector<double>(_size);
    }
    
    if (x == NULL){
        x = new std::vector<double>(_size);
    }
    
    
    A->zero();
    std::fill(b->begin(), b->end(), 0);
    std::fill(x->begin(), x->end(), 0);
}

void Projection::fillA(){
    assert(*_phi->grid == *_u);
    int row = 0;

    //Iterera över all celler
    for (GridFieldIterator<double> it = _phi->grid->iterator(); !it.done(); it.next()) {
        int i,j,k;
        it.index(i, j, k);
        
        if (k == 0) {
            
            CellType centerCell = Fire::getCellType(_phi->grid->valueAtIndex(it.index()));
            
            if (centerCell == _cellType && !isSolid(i, j, k)) {
                
                double scale = _dt/( _dx*_dx);
       
                
                if(_phi->grid->isValid(i-1, j, k)){
                    CellType left = Fire::getCellType(_phi->grid->valueAtIndex(i-1, j, k));
                    
                    if (isSolid(i-1, j, k)) {
                        //Solid
                    }else if (left == _cellType) {
                        A->set_element(row, _phi->grid->indexAt(i-1, j, k), -scale/(getDensity(i, j, k, LEFT)));
                        A->add_to_element(row, row, scale/(getDensity(i, j, k, LEFT)));
                    }else{
                        A->add_to_element(row, row, scale/(getDensity(i, j, k, LEFT)));
                    }
                }else{
                    A->add_to_element(row, row, scale/(getDensity(i, j, k, LEFT)));
                }
                
                if(_phi->grid->isValid(i+1, j, k)){
                    CellType right = Fire::getCellType(_phi->grid->valueAtIndex(i+1, j, k));
                    if (isSolid(i+1, j, k)) {
                        //Solid
                    }else if (right == _cellType) {
                        A->set_element(row, _phi->grid->indexAt(i+1, j, k), -scale/(getDensity(i, j, k, RIGHT)));
                        A->add_to_element(row, row, scale/(getDensity(i, j, k, RIGHT)));
                    }else{
                        A->add_to_element(row, row, scale/(getDensity(i, j, k, RIGHT)));
                    }
                }else{
                    A->add_to_element(row, row, scale/(getDensity(i, j, k, RIGHT)));
                }
                
                if(_phi->grid->isValid(i, j-1, k)){
                    CellType down = Fire::getCellType(_phi->grid->valueAtIndex(i, j-1, k));
                    if (isSolid(i, j-1, k)) {
                        //Solid
                    }else if (down == _cellType) {
                        A->set_element(row, _phi->grid->indexAt(i, j-1, k), -scale/(getDensity(i, j, k, DOWN)));
                        A->add_to_element(row, row, scale/(getDensity(i, j, k, DOWN)));
                    }else{
                        A->add_to_element(row, row, scale/(getDensity(i, j, k, DOWN)));
                    }
                }else{
                    A->add_to_element(row, row, scale/(getDensity(i, j, k, DOWN)));
                }
                
                if(_phi->grid->isValid(i, j+1, k)){
                    CellType up = Fire::getCellType(_phi->grid->valueAtIndex(i, j+1, k));
                    if (isSolid(i, j+1, k)) {
                        //Solid
                    }else if (up == _cellType) {
                        A->set_element(row, _phi->grid->indexAt(i, j+1, k), -scale/(getDensity(i, j, k, UP)));
                        A->add_to_element(row, row, scale/(getDensity(i, j, k,UP)));
                    }else{
                        A->add_to_element(row, row, scale/(getDensity(i, j, k, UP)));
                    }
                }else{
                    A->add_to_element(row, row, scale/(getDensity(i, j, k, UP)));
                }
                
            }
            
            row++;
        }
    }
    
}


bool Projection::isSolid(int i,int j ,int k) const{
    return _solids->valueAtIndex(i,j,k);
}


void Projection::fillb(){
    assert(*_phi->grid == *_u);
    double mean = 0;
    //Fill b
    for (GridFieldIterator<double> it = _phi->grid->iterator(); !it.done(); it.next()) {
        int i,j,k;
        it.index(i, j, k);
        int index = it.index();
        CellType cellType = Fire::getCellType(_phi->grid->valueAtIndex(i, j, k));

        if (_cellType == cellType && !isSolid(i,j,k)) {
            double scale = 1.0/_dx;
            double right = !isSolid(i+1,j,k) ? _u->valueAtFace(i, j, k, RIGHT) : 0.0;
            double left = !isSolid(i-1,j,k) ?_u->valueAtFace(i, j, k, LEFT) : 0.0;
            double up = !isSolid(i,j+1,k) ? _u->valueAtFace(i, j, k, UP) : 0.0;
            double down = !isSolid(i,j-1,k) ?_u->valueAtFace(i, j, k, DOWN) : 0.0;
                
            double div = -scale*(right-left+up-down);
            mean += div;
            (*b)[index] = div;
        }else{
            (*b)[index] = 0;
        }
    }
    
    /*mean /= static_cast<double>( (*b).size() );
    //SUbstract mean...
    for (int i = 0;  i < (*b).size(); i++) {
        (*b)[i] -= mean;
    }*/
}

void Projection::print_vector(std::vector<double> &v) const{
    std::cout << "[";
    for (int i = 0; i < v.size(); i++) {
        //std::cout << "|" << std::setw(5) << val << "|" << std::endl;
        std::cout << std::setw(5) << v[i] << ";" << std::endl;
    }
    std::cout << "]";
}

void Projection::applyPressure(){
    for (GridFieldIterator<double> it = _phi->grid->iterator(); !it.done(); it.next()) {
        
        int i,j,k;
        it.index(i, j, k);
        int index = it.index();
        CellType cellType = Fire::getCellType(_phi->grid->valueAtIndex(i, j, k));

        if (k == 0 && cellType == _cellType && !isSolid(i,j,k)) {
            double scale = _dt/(_dx);
            
            _u->addValueAtFace(-scale*(*x)[index]/getDensity(i, j, k, LEFT), i, j, k, LEFT);
            _u->addValueAtFace(scale*(*x)[index]/getDensity(i, j, k, RIGHT), i, j, k, RIGHT);
            _u->addValueAtFace(-scale*(*x)[index]/getDensity(i, j, k, DOWN), i, j, k, DOWN);
            _u->addValueAtFace(scale*(*x)[index]/getDensity(i, j, k, UP), i, j, k, UP);
            

        }

    }
    
}


void Projection::project(MACGrid *u,const GridField<bool> *solids,CellType cellType,double dt){
    assert(*u == *_u);
    _u = u;
    _solids = solids;
    _cellType = cellType;
    
    //Compute size
    _size = _phi->grid->size();
    _dx = _u->dx();
    _dt = dt;

    resize(); //Sätt storlekar på arrayer/matriser
    fillA(); //Fyll A-matrisen
    fillb(); //Fyll b-vektorn
    /*
    for (int i = 0;  i < _size; i++) {
        std::cout << "|";
        double row_sum = 0;
        for (int j = 0; j < _size; j++) {
            row_sum +=  (*A)(i,j) ;
            std::cout << " " << std::setw(5) << std::setprecision(3) << (*A)(i,j) ;
        }
        std::cout << " |" << std::endl;
    }*/

    
    
    
    
    //Divergence
    
    //Border condition
    for (GridFieldIterator<bool> it = solids->iterator(); !it.done(); it.next()) {
        if (it.value()) {
            int i,j,k;
            it.index(i, j, k);
            _u->setValueAtFace(0, i, j, k, LEFT);
            _u->setValueAtFace(0, i, j, k, RIGHT);
            _u->setValueAtFace(0, i, j, k, UP);
            _u->setValueAtFace(0, i, j, k, DOWN);
        }
    }
    
    fillb(); //Fyll b-vektorn
    //print_vector(*b);
    //A->write_matlab(std::cout, "A");
    double sum = 0;
    for (int i = 0; i< b->size(); i++) {
        sum += b->at(i);
    }
   
    //Border condition
    for (GridFieldIterator<bool> it = solids->iterator(); !it.done(); it.next()) {
        if (it.value()) {
            int i,j,k;
            it.index(i, j, k);
            _u->setValueAtFace(0, i, j, k, LEFT);
            _u->setValueAtFace(0, i, j, k, RIGHT);
            _u->setValueAtFace(0, i, j, k, UP);
            _u->setValueAtFace(0, i, j, k, DOWN);
        }
    }

    std::cout << "divergence before projection: " << sum << std::endl;
    
    double residual;
    int iterations;
    PCGSolver<double> solver;
    if(solver.solve(*A, *b, *x, residual, iterations)){
    
        /*
        A->write_matlab(std::cout, "A1");
        std::cout << "b = [";
        for (int i = 0; i < b->size(); i++) {
            std::cout << " " << b->at(i);
        }
        std::cout << "]';";
        */
        std::cout << "YAY!" << std::endl;

    }else{
        /*
        A->write_matlab(std::cout, "A2");
        std::cout << "b = [";
        for (int i = 0; i < b->size(); i++) {
            std::cout << " " << b->at(i);
        }
        std::cout << "]';";
        */
        //throw;
        std::cout << "FAIL! resudual:" << residual << std::endl;
        
    }


    
    
    applyPressure();
    

    //Border condition
    for (GridFieldIterator<bool> it = solids->iterator(); !it.done(); it.next()) {
        int i,j,k;
        it.index(i, j, k);
        if (it.value()) {

            //if (it.value()) {
            
            _u->setValueAtFace(0, i, j, k, LEFT);
            _u->setValueAtFace(0, i, j, k, RIGHT);
            _u->setValueAtFace(0, i, j, k, UP);
            _u->setValueAtFace(0, i, j, k, DOWN);
            //}
        }
    }
    
    //Divergence
    fillb(); //Fyll b-vektorn
    std::cout << "fillb:" << std::endl;
    //print_vector(*b);
     sum = 0;
    for (int i = 0; i< b->size(); i++) {
        sum += b->at(i);
    }
    
    std::cout << "divergence after projection: " << sum << std::endl;
    
    
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

    //return FirePresets::rhof;
	// sid. 104 (Bridson)
    double alfa = getAlpha(i, j,k, d);
    return FirePresets::rhof*alfa+(1.0-alfa)*FirePresets::rhob;
    
}

