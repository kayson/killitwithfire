//
//  Projection2.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/22/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FuidFire__Projection3D__
#define __FuidFire__Projection3D__

#include "../MACGrid.h"
#include "pcgsolver/pcg_solver.h"
#include "LevelSet.h"
#include "GhostMAC.h"
#include "GridField.hpp"

class Projection3D {
private:
    void print_vector(std::vector<double> &v) const;
private:
    void resize();
    void fillA();
    void fillb();
    double div(int i ,int j ,int k,DirectionEnums d, CellType centerCellType);
    double densityAt(int i,int j,int k,DirectionEnums direction);
    double getAlpha( int i,  int j,  int k, DirectionEnums d) const;
    double getDensity( int i,  int j,  int k, DirectionEnums d) const;
    void applyPressure();
    bool isSolid(int i,int j,int k) const;
private:
    MACGrid *_u;
    LevelSet *_phi;
    const GridField<bool> *_solids;
    CellType _cellType;
    int _size;
    double _dx;
    double _dt;
    SparseMatrix<double> *A;
    std::vector<double> *b;
    std::vector<double> *x;
    GridField<double> *p;
public:
    Projection3D(MACGrid *u, LevelSet *phi):_u(u),_phi(phi){
        A = nullptr;
        b = nullptr;
        x = nullptr;
    };
    
    ~Projection3D(){
        delete A;
        delete b;
        delete x;
    };
    void project(MACGrid *u,const GridField<bool> *solids,CellType cellType, double dt);
};
#endif /* defined(__FuidFire__Projection2__) */
