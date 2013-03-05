//
//  Projection2.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/22/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FuidFire__Projection2__
#define __FuidFire__Projection2__

#include "../MACGrid.h"
#include "../Pressure/pcgsolver/pcg_solver.h"
class PCGProjection2D {
private:
    virtual void resize();
    virtual void fillA();
    virtual void fillb();
    virtual void applyPressure();
private:
    MACGrid *_u;
    GridField<int> *_cellType;
    
    int _size;
    double _dx;
    double _dt;
    double _rho;
    SparseMatrix<double> *A;
    std::vector<double> *b;
    std::vector<double> *x;
public:
    PCGProjection2D(){
        A = nullptr;
        b = nullptr;
        x = nullptr;
        _u = nullptr;
        _cellType = nullptr;
    }
    PCGProjection2D(MACGrid *u, GridField<int> *cellType):_u(u),_cellType(cellType){
        A = nullptr;
        b = nullptr;
        x = nullptr;
    };
    ~PCGProjection2D(){
        delete A;
        delete b;
        delete x;    
    };
    virtual void project(double dt,double rho);
};
#endif /* defined(__FuidFire__Projection2__) */
