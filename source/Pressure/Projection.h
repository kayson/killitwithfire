//
//  Projection2.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/22/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FuidFire__Projection__
#define __FuidFire__Projection__

#include "../MACGrid.h"
#include "pcgsolver/pcg_solver.h"
#include "LevelSet.h"
class Projection {
private:
    void resize();
    void fillA();
    void fillb();
    double div(int i ,int j ,int k,DirectionEnums d, CellType centerCellType);
    double densityAt(int i,int j,int k,DirectionEnums direction);
    double getAlpha(const int i, const int j, const int k, DirectionEnums d);
    double getDensity(const int i, const int j, const int k, DirectionEnums d);
    void applyPressure();
private:
    MACGrid *_u;
    LevelSet *_phi;
    
    int _size;
    double _dx;
    double _dt;
    SparseMatrix<double> *A;
    std::vector<double> *b;
    std::vector<double> *x;
    GridField<double> *p;
public:
    Projection(MACGrid *u, LevelSet *phi):_u(u),_phi(phi){
        A = nullptr;
        b = nullptr;
        x = nullptr;
    };

    ~Projection(){
        delete A;
        delete b;
        delete x;    
    };
    void project(double dt);
};
#endif /* defined(__FuidFire__Projection2__) */
