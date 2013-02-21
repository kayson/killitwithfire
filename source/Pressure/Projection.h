//
//  Projection.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/19/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FuidFire__Projection__
#define __FuidFire__Projection__
#include "../MACGrid.h"
#include "../GridField.h"

#ifdef __APPLE__
#include "pcg_solver.h"
#elif defined _WIN32 || defined _WIN64
#include "pcgsolver\pcg_solver.h"
#endif

#include <iostream>
#include <algorithm>
class Projection {
public:
    virtual ~Projection();
    virtual void project() = 0;
};


class PCGProjection : public Projection {
private:
    MACGrid *_u;
    GridField<int> *_cellType;
    
    //Variables to solve
    SparseMatrix<double> A;
    std::vector<double> b;
    std::vector<double> x;
public:
    double residual;
    int iterations;
private:
    //Hide default constructor
    PCGProjection();
    //Private functions
    void fillA(){
        /*for (GridFieldIterator<double> it = _grid->_center->iter) {
            
        }*/
    };
    void fillb(){
        
    };
public:
    PCGProjection(MACGrid &grid, GridField<int> &cellType){
        _u = &grid;
        _cellType = &cellType;
        A = SparseMatrix<double>(_u->size());
        b = std::vector<double>(_u->size());
        x = std::vector<double>(_u->size());
        std::fill(x.begin(),x.end(),0);
        iterations = 100;
        residual = 0.01;
    };
    virtual ~PCGProjection(){};
    virtual void project(){
        fillA(); //Fyll A-matrisen
        fillb(); //Fyll b-matrisen
        PCGSolver<double> solver;
        solver.solve(A, b, x, residual, iterations); //Gör magi
        
    };
    

};



#endif /* defined(__FuidFire__Projection__) */
