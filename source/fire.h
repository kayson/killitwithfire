

#ifndef FIRE_H
#define FIRE_H


#include "presets/firePresets.h"

#include "VelocityField.h"
#include "levelset/LevelSet.h"
#include "GridField.h"
#include "GridMapping.h"
#include "helper.h"
#include "Pressure\pcgsolver\sparse_matrix.h"

#include "armadillo/armadillo"

class Fire{
protected:
    double computeDT(double currentTime);
	void advectLevelSet(double duration);

	void project(double duration);
	double getAlpha(const int i, const int j, const int k, DirectionEnums d);
	double getDensity(const int i, const int j, const int k, DirectionEnums d);

	void computeCellTypes();
	CellType getCellType(const int i, const int j, const int k);

	//void 
public:
	Fire(FirePresets *preset);
	~Fire();

    void runSimulation();

	void drawCenterVelocities();
	void draw();

private:

    FirePresets *preset;
    LevelSet phi;

	//Pressure field
	GridField<double> *p;
	GridField<double> *rhs;

	// Sparse matrix A
	SparseMatrix<double> *A;

	// Border conditions
	BorderCondition *_borderCondition;

    //Grid
    MACGrid u;

	GridField<int> celltype;
};

#endif //FIRE_H
