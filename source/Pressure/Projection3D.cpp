//
//  Projection2.cpp
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/22/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#include "Projection3D.h"
#include <iomanip>
#include "glfw.h"
#include "pcg_solver.h"

void PCGProjection3D::resize(){
    //Compute size
    
    _size = _cellType->size();

    
    if(A == nullptr ){
        A = new SparseMatrix<double>(_size,7);
    }
    A = new SparseMatrix<double>(_size,7);
    
    if (b == nullptr){
        b = new std::vector<double>(_size);
    }
    b = new std::vector<double>(_size);
    
    
    if (x == nullptr){
        x = new std::vector<double>(_size);
    }
    
    A->resize(_size);
    b->resize(_size);
    x->resize(_size);
}

void PCGProjection3D::fillA(){
    assert(*_cellType == *_u);
    double scale = _dt/(_rho*_dx*_dx);
    int row = 0;
    for (GridFieldIterator<int> it = _cellType->iterator(); !it.done(); it.next()) {
        
        int i,j,k;
        it.index(i, j, k);

        
            if(_cellType->valueAtIndex(i, j, k) == IGNITED) {
                
                if (_cellType->valueAtIndex(i+1,j,k) == IGNITED && _cellType->isValid(i+1, j, k)) {
                    A->add_to_element(row, row, scale);
                    A->set_element(row,_cellType->indexAt(i+1, j, k),  -scale);
                }else if (_cellType->valueAtIndex(i+1,j,k) == AIR && _cellType->isValid(i+1, j, k)){
                    A->add_to_element(row, row, scale);
                }
                
                
                if (_cellType->valueAtIndex(i-1,j,k) == IGNITED && _cellType->isValid(i-1, j, k)) {
                    A->add_to_element(row, row, scale);
                    A->set_element( row,_cellType->indexAt(i-1, j, k), -scale);
                }else if (_cellType->valueAtIndex(i-1,j,k) == AIR && _cellType->isValid(i-1, j, k)){
                    A->add_to_element(row, row, scale);
                }
                
                if (_cellType->valueAtIndex(i,j+1,k) == IGNITED && _cellType->isValid(i, j+1, k)) {
                    A->add_to_element(row, row, scale);
                    A->set_element( row,_cellType->indexAt(i, j+1, k), -scale);
                }else if (_cellType->valueAtIndex(i,j+1,k) == AIR && _cellType->isValid(i, j+1, k)){
                    A->add_to_element(row, row, scale);
                }
                
                if (_cellType->valueAtIndex(i,j-1,k) == IGNITED &&  _cellType->isValid(i, j-1, k)) {
                    A->add_to_element(row, row, scale);
                    A->set_element( row,_cellType->indexAt(i, j-1, k), -scale);
                }else if (_cellType->valueAtIndex(i,j-1,k) == AIR &&  _cellType->isValid(i, j-1, k)){
                    A->add_to_element(row, row, scale);
                }
                
                if (_cellType->valueAtIndex(i,j,k-1) == IGNITED &&  _cellType->isValid(i, j, k-1)) {
                    A->add_to_element(row, row, scale);
                    A->set_element( row,_cellType->indexAt(i, j, k-1), -scale);
                }else if (_cellType->valueAtIndex(i,j,k-1) == AIR &&  _cellType->isValid(i, j, k-1)){
                    A->add_to_element(row, row, scale);
                }
                
                if (_cellType->valueAtIndex(i,j,k+1) == IGNITED &&  _cellType->isValid(i, j, k+1)) {
                    A->add_to_element(row, row, scale);
                    A->set_element( row,_cellType->indexAt(i, j, k+1), -scale);
                }else if (_cellType->valueAtIndex(i,j,k+1) == AIR &&  _cellType->isValid(i, j, k+1)){
                    A->add_to_element(row, row, scale);
                }
                
            }

            row++;

        }

    
}

void PCGProjection3D::fillb(){
    
    
    //Fillb
    double scale = _dt/(_rho*_dx);
    int index = 0;
    for (GridFieldIterator<int> it = _cellType->iterator(); !it.done(); it.next()) {
        
        int i,j,k;
        it.index(i, j, k);

        if (_cellType->valueAtIndex(i, j, k) == IGNITED){
            double div = -scale*(_u->valueAtFace(i, j, k, RIGHT)-_u->valueAtFace(i, j, k, LEFT)
                                 +_u->valueAtFace(i, j, k, UP)-_u->valueAtFace(i, j, k, DOWN)
                                 +_u->valueAtFace(i, j, k, BACKWARD)-_u->valueAtFace(i, j, k, FORWARD));
            (*b)[index] = div;
        }else{
            (*b)[index] = 0;
        }
        index++;
    }

}
void PCGProjection3D::applyPressure(){
    double scale = _dt/(_rho*_dx);
    int index = 0;
    for (GridFieldIterator<int> it = _cellType->iterator(); !it.done(); it.next()) {
        
        int i,j,k;
        it.index(i, j, k);

        if (_cellType->valueAtIndex(i, j, k) == IGNITED) {
                _u->addValueAtFace(-scale*(*x)[index], i, j, k, LEFT);
                _u->addValueAtFace(scale*(*x)[index], i, j, k, RIGHT);
                _u->addValueAtFace(-scale*(*x)[index], i, j, k, DOWN);
                _u->addValueAtFace(scale*(*x)[index], i, j, k, UP);
                _u->addValueAtFace(-scale*(*x)[index], i, j, k, FORWARD);
                _u->addValueAtFace(scale*(*x)[index], i, j, k, BACKWARD);
                
                
                //double c_x,y,z;
                //_u->indexToWorld(i, j, k, c_x, y, z);
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


void PCGProjection3D::project(double dt,double rho){
    _dx = _u->dx();
    _dt = dt;
    _rho = rho;
    
    resize(); //Sätt storlekar på arrayer/matriser
    fillA(); //Fyll A-matrisen
    fillb(); //Fyll b-matrisen

    double residual;
    int iterations;
    PCGSolver<double> solver;
    solver.solve(*A, *b, *x, residual, iterations); //Gör magi...
    applyPressure();
}