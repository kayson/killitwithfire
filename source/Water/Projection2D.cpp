//
//  Projection2.cpp
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/22/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#include "Projection2D.h"
#include <iomanip>
#include "glfw.h"
#include "../Pressure/pcgsolver/pcg_solver.h"

#define nullptr NULL

void PCGProjection2D::resize(){
    //Compute size
    int size = 0;
    for (GridFieldIterator<int> it = _cellType->iterator(); !it.done(); it.next()) {
        int i,j,k;
        it.index(i, j, k);
        if (k == 0) {
            size++;
        }
    }
    
    if(A == nullptr ){
        A = new SparseMatrix<double>(size,5);
    }
    A = new SparseMatrix<double>(size,5);
    
    if (b == nullptr){
        b = new std::vector<double>(size);
    }
    b = new std::vector<double>(size);
    
    
    if (x == nullptr){
        x = new std::vector<double>(size);
    }
    _size = size;
    A->resize(size);
    b->resize(size);
    x->resize(size);
}

void PCGProjection2D::fillA(){
    assert(*_cellType == *_u);
    double scale = _dt/(_rho*_dx*_dx);
    int row = 0;
    for (GridFieldIterator<int> it = _cellType->iterator(); !it.done(); it.next()) {
        
        int i,j,k;
        it.index(i, j, k);
        if (k == 0){
            
            if(_cellType->valueAtIndex(i, j, k) == IGNITED) {
                
                if (_cellType->valueAtIndex(i+1,j,k) == IGNITED && _cellType->isValid(i+1, j, k)) {
                    A->add_to_element(row, row, scale);
                    A->set_element(row,_cellType->indexAt(i+1, j, k),  -scale);
                }else if (_cellType->valueAtIndex(i+1,j,k) == AIR){
                    A->add_to_element(row, row, scale);
                }
                
                
                if (_cellType->valueAtIndex(i-1,j,k) == IGNITED && _cellType->isValid(i-1, j, k)) {
                    A->add_to_element(row, row, scale);
                    A->set_element( row,_cellType->indexAt(i-1, j, k), -scale);
                }else if (_cellType->valueAtIndex(i-1,j,k) == AIR){
                    A->add_to_element(row, row, scale);
                }
                
                if (_cellType->valueAtIndex(i,j+1,k) == IGNITED && _cellType->isValid(i, j+1, k)) {
                    A->add_to_element(row, row, scale);
                    A->set_element( row,_cellType->indexAt(i, j+1, k), -scale);
                }else if (_cellType->valueAtIndex(i,j+1,k) == AIR){
                    A->add_to_element(row, row, scale);
                }
                
                if (_cellType->valueAtIndex(i,j-1,k) == IGNITED &&  _cellType->isValid(i, j-1, k)) {
                    A->add_to_element(row, row, scale);
                    A->set_element( row,_cellType->indexAt(i, j-1, k), -scale);
                }else if (_cellType->valueAtIndex(i,j-1,k) == AIR){
                    A->add_to_element(row, row, scale);
                }
                /*
                CellType center = static_cast<CellType>(_cellType->valueAtIndex(i,j,k));
                CellType left = static_cast<CellType>(_cellType->valueAtIndex(i-1,j,k));
                CellType right = static_cast<CellType>(_cellType->valueAtIndex(i+1,j,k));
                CellType up = static_cast<CellType>(_cellType->valueAtIndex(i,j+1,k));
                CellType down = static_cast<CellType>(_cellType->valueAtIndex(i,j-1,k));
                if (up == AIR) {
                    std::cout << "    ~    " << std::endl;
                }else if (up == SOLID){
                    std::cout << "    #    " << std::endl;
                }else if (up == IGNITED){
                    std::cout << "    @    " << std::endl;
                }
                
                if (left == AIR) {
                    std::cout << "~   ";
                }else if (left == SOLID){
                    std::cout << "#   ";
                }else if (left == IGNITED){
                    std::cout << "@   ";
                }
                
                if (center == AIR) {
                    std::cout << "~";
                }else if (center == SOLID){
                    std::cout << "#";
                }else if (center == IGNITED){
                    std::cout << "@";
                }
                
                if (right == AIR) {
                    std::cout << "   ~";
                }else if (right == SOLID){
                    std::cout << "   #";
                }else if (right == IGNITED){
                    std::cout << "   @";
                }
                
                if (down == AIR) {
                    std::cout << std::endl << "    ~    ";
                }else if (down == SOLID){
                    std::cout << std::endl << "    #    ";
                }else if (down == IGNITED){
                    std::cout << std::endl << "    @    ";
                }
                std::cout << std::endl;
                
                */
                /*double x,y,z;
                 _cellType->indexToWorld(i, j, k, x, y, z);
                 glColor3d(0.0,1.0,0.0);
                 glBegin(GL_LINE_STRIP);
                 glVertex3d(x, y, 0);
                 glVertex3d(x + 0.1, y , 0);
                 glEnd();*/
                
            }
            row++;

        }

    }
}

void PCGProjection2D::fillb(){
    
    
    //Fillb
    double scale = _dt/(_rho*_dx);
    int index = 0;
    double mean = 0;
    for (GridFieldIterator<int> it = _cellType->iterator(); !it.done(); it.next()) {
        
        int i,j,k;
        it.index(i, j, k);
        if (k == 0){
            if (_cellType->valueAtIndex(i, j, k) == IGNITED){
                    double div = -scale*(_u->valueAtFace(i, j, k, RIGHT)-_u->valueAtFace(i, j, k, LEFT)+_u->valueAtFace(i, j, k, UP)-_u->valueAtFace(i, j, k, DOWN));
                    (*b)[index] = div;
                    mean += div;
                }else{
                    (*b)[index] = 0;
                }
                index++;
        }
    }
    
    
    
}
void PCGProjection2D::applyPressure(){
    double scale = _dt/(_rho*_dx);
    int index = 0;
    for (GridFieldIterator<int> it = _cellType->iterator(); !it.done(); it.next()) {
        
        int i,j,k;
        it.index(i, j, k);
        if (k == 0){
            if (_cellType->valueAtIndex(i, j, k) == IGNITED) {
                _u->addValueAtFace(-scale*(*x)[index], i, j, k, LEFT);
                _u->addValueAtFace(scale*(*x)[index], i, j, k, RIGHT);
                _u->addValueAtFace(-scale*(*x)[index], i, j, k, DOWN);
                _u->addValueAtFace(scale*(*x)[index], i, j, k, UP);
                
                
                double c_x,y,z;
                _u->indexToWorld(i, j, k, c_x, y, z);
                /*glBegin(GL_QUADS);
                //glColor3d(scale*(*x)[index]*1000000.0,scale*(*x)[index], scale*(*x)[index]);
                
                if (scale*(*x)[index] > 0) {
                    
                }
                
                std::cout << scale*(*x)[index] << std::endl;
                glColor3d(1,0,1);
                glVertex3d(c_x-_dx*0.5, y-_dx*0.5, 0);
                glVertex3d(c_x+_dx*0.5, y-_dx*0.5, 0);
                glVertex3d(c_x+_dx*0.5, y+_dx*0.5, 0);
                glVertex3d(c_x-_dx*0.5, y+_dx*0.5, 0);
                glEnd();
                */
            }
            index++;
        }
    }
}


void PCGProjection2D::project(double dt,double rho){
    _dx = _u->dx();
    _dt = dt;
    _rho = rho;
    
    resize(); //Sätt storlekar på arrayer/matriser
    fillA(); //Fyll A-matrisen
    fillb(); //Fyll b-matrisen

    double residual;
    int iterations;
    PCGSolver<double> solver;
    //solver.set_solver_parameters(1e-5, 100);
    solver.solve(*A, *b, *x, residual, iterations); //Gör magi...
    applyPressure();
}