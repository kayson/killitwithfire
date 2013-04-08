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
    
    if(A != nullptr ){
        delete A;
    }
    A = new SparseMatrix<double>(_size,5);
    
    
    if (b != nullptr){
        delete b;
    }
    b = new std::vector<double>(_size);
    std::fill(b->begin(), b->end(), 0);
    
    if (x != nullptr){
        delete x;
    }
    x = new std::vector<double>(_size);
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
            
            CellType center = Fire::getCellType(_phi->grid->valueAtIndex(it.index()));
            bool solid = isSolid(i, j, k);
            if (center == FUEL && !solid) {
                
                double scale = _dt/(_dx*_dx);
                if (center == FUEL) {
                    scale /= FirePresets::rhof;
                }else if (center == BURNT){
                    scale /= FirePresets::rhob;
                }else{
                    //Do nothing
                }
                
                if(_phi->grid->isValid(i-1, j, k)){
                    CellType left = Fire::getCellType(_phi->grid->valueAtIndex(i-1, j, k));
                    if ((left == BURNT || left == FUEL)) {
                        A->set_element(row, _phi->grid->indexAt(i-1, j, k), -scale);
                        A->add_to_element(row, row, scale);
                    }//Solid
                }
                
                if(_phi->grid->isValid(i+1, j, k)){
                    CellType right = Fire::getCellType(_phi->grid->valueAtIndex(i+1, j, k));
                    if (right == BURNT || right == FUEL) {
                        A->set_element(row, _phi->grid->indexAt(i+1, j, k), -scale);
                        A->add_to_element(row, row, scale);
                    }
                }
                
                if(_phi->grid->isValid(i, j-1, k)){
                    CellType down = Fire::getCellType(_phi->grid->valueAtIndex(i, j-1, k));
                    if (down == BURNT || down == FUEL) {
                        A->set_element(row, _phi->grid->indexAt(i, j-1, k), -scale);
                        A->add_to_element(row, row, scale);
                    }
                }
                if (j == 0) {
                    A->add_to_element(row, row, scale);
                }else if(_phi->grid->isValid(i, j+1, k)){
                    CellType up = Fire::getCellType(_phi->grid->valueAtIndex(i, j+1, k));
                    if (up == BURNT || up == FUEL) {
                        A->set_element(row, _phi->grid->indexAt(i, j+1, k), -scale);
                        A->add_to_element(row, row, scale);
                    }
                }
                
            }
            
            row++;
        }
    }
    
    
    /*
    int row = -1;
    
    for (GridMappingIterator it = _u->iterator(); !it.done(); it.next()) {
        int i,j,k;
        it.index(i, j, k);
        if (k == 0) {
            row++;
            if (!(i == 0 || i == (_phi->grid->xdim()-1) || j == (_phi->grid->ydim()-1))
                && Fire::getCellType(_phi->grid->valueAtIndex(i, j, k)) == FUEL) {
                //A.k.a Solid...
                if (i+1 == (_phi->grid->xdim()-1)) {
                    
                }else if (_phi->grid->isValid(i+1, j, k)) {
                    double scale = _dt/(_dx*_dx);
                    int col = _phi->grid->indexAt(i+1, j, k);
                    A->set_element(row, col, -scale/getDensity(i, j, k, RIGHT));
                    
                    
                    if (Fire::getCellType(_phi->grid->valueAtIndex(i, j, k)) == BURNT) {
                        A->add_to_element(row, row, scale/FirePresets::rhof);
                    }else{
                        A->add_to_element(row, row, scale/FirePresets::rhof);
                    }
                    
                }
                
                if (i-1 == 0) {
                    //A.k.a Solid...
                }else if (_phi->grid->isValid(i-1, j, k)) {
                    double scale = _dt/(_dx*_dx);
                    A->set_element(row, _phi->grid->indexAt(i-1, j, k), -scale/getDensity(i, j, k, LEFT));
                    
                    if (Fire::getCellType(_phi->grid->valueAtIndex(i, j, k)) == BURNT) {
                        A->add_to_element(row, row, scale/FirePresets::rhof);
                    }else{
                        A->add_to_element(row, row, scale/FirePresets::rhof);
                    }
                    
                }
                
                if (j+1 == (_phi->grid->ydim()-1)) {
                    //A.k.a Solid...
                }else if(_phi->grid->isValid(i, j+1, k)){
                    double scale = _dt/(_dx*_dx);
                    A->set_element(row, _phi->grid->indexAt(i, j+1, k), -scale/getDensity(i, j, k, UP));
                    
                    if (Fire::getCellType(_phi->grid->valueAtIndex(i, j, k)) == BURNT) {
                        A->add_to_element(row, row, scale/FirePresets::rhof);
                    }else{
                        A->add_to_element(row, row, scale/FirePresets::rhof);
                    }
                }
                
                if (j-1 == 0) {
                    //A.k.a Solid...
                }else if(_phi->grid->isValid(i, j-1, k)) {
                    double scale = _dt/(_dx*_dx);
                    A->set_element(row, _phi->grid->indexAt(i, j-1, k), -scale/getDensity(i, j, k, DOWN));
                    
                    if (Fire::getCellType(_phi->grid->valueAtIndex(i, j, k)) == BURNT) {
                A->add_to_element(row, row, scale/FirePresets::rhof);
                    }else{
                        A->add_to_element(row, row, scale/FirePresets::rhof);
                    }
                    
                }
            }
        }
    }
     */
}


bool Projection::isSolid(int i,int j ,int k) const{
    return (i == 0 || i == (_phi->grid->xdim()-1)
         || j == 0 );
}


void Projection::fillb(){
    assert(*_phi->grid == *_u);
    double mean = 0;
    //Fill b
    for (GridFieldIterator<double> it = _phi->grid->iterator(); !it.done(); it.next()) {
        int i,j,k;
        it.index(i, j, k);
        int index = it.index();

        if (k == 0) {
            CellType cellType =  Fire::getCellType(_phi->grid->valueAtIndex(i, j, k));
            
            if (cellType == FUEL && !isSolid(i,j,k)) {
                double scale = 1.0/_dx;
                double right = !isSolid(i+1,j,k) ? _w->valueAtFace(i, j, k, RIGHT,cellType)*scale : 0.0;
                double left = !isSolid(i-1,j,k) ?_w->valueAtFace(i, j, k, LEFT,cellType)*scale : 0.0;
                double down = !isSolid(i,j+1,k) ? _w->valueAtFace(i, j, k, DOWN,cellType)*scale : 0.0;
                double up = !isSolid(i,j-1,k) ?_w->valueAtFace(i, j, k, UP,cellType)*scale : 0.0;
                
                double div = -scale*((right-left)+(up-down));
                mean += div;
                (*b)[index] = div;
            }else{
                (*b)[index] = 0;
            }

        }
    }
    mean /= static_cast<double>((*b).size());
    //SUbstract mean...
    for (int i = 0;  i < (*b).size(); i++) {
        (*b)[i] -= mean;
    }
    
}

void Projection::applyPressure(){
    

    
    for (GridFieldIterator<double> it = _phi->grid->iterator(); !it.done(); it.next()) {
        
        int i,j,k;
        it.index(i, j, k);
        int index = it.index();
        
        if (k == 0 && !isSolid(i,j,k)) {
            CellType cellType = Fire::getCellType(_phi->grid->valueAtIndex(i, j, k));
            if (cellType == FUEL) {
                double scale;
                if (cellType == BURNT) {
                    scale = _dt/(_dx*FirePresets::rhob);
                }else{// if (cellType == FUEL){
                    scale = _dt/(_dx*FirePresets::rhof);
                }
                
                _u->addValueAtFace(-scale*(*x)[index], i, j, k, LEFT);
                _u->addValueAtFace(scale*(*x)[index], i, j, k, RIGHT);
                _u->addValueAtFace(-scale*(*x)[index], i, j, k, DOWN);
                _u->addValueAtFace(scale*(*x)[index], i, j, k, UP);
            }
        }
    }
    
    
    /*
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
     */
    
    
    
    
}


void Projection::project(double dt){
    assert(*_phi->grid == *_u);
    
    //Compute size
    _size = _phi->grid->size();
    _dx = _u->dx();
    _dt = dt;
    std::cout << _size << std::endl;
    resize(); //Sätt storlekar på arrayer/matriser
    fillA(); //Fyll A-matrisen
    fillb(); //Fyll b-vektorn

    //Divergence
    fillb(); //Fyll b-vektorn
    double sum = 0;
    for (int i = 0; i< b->size(); i++) {
        sum += b->at(i);
    }
    
    std::cout << "divergence before projection: " << sum << std::endl;
    
    double residual;
    int iterations;
    PCGSolver<double> solver;
    
    if(solver.solve(*A, *b, *x, residual, iterations)){
    
        /*A->write_matlab(std::cout, "A1");
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
        std::cout << "FAIL!" << std::endl;
        
    }
    applyPressure();
    

    
    
    for (GridFieldIterator<double> it = _phi->grid->iterator(); !it.done(); it.next()) {
        int i,j,k;
        it.index(i, j, k);
        if (isSolid(i, j, k)) {
            _u->setValueAtFace(0, i, j, k, LEFT);
            _u->setValueAtFace(0, i, j, k, RIGHT);
            _u->setValueAtFace(0, i, j, k, UP);
            _u->setValueAtFace(0, i, j, k, DOWN);
        }
    }
    
    //Divergence
    fillb(); //Fyll b-vektorn
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
    return FirePresets::rhof;
	// sid. 104 (Bridson)
    double alfa = getAlpha(i, j,k, d);
    return FirePresets::rhof*alfa+(1.0-alfa)*FirePresets::rhob;
}

