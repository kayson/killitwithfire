

#ifndef FIRE_H
#define FIRE_H


#include "presets/firePresets.h"

#include "levelset/LevelSet.h"
#include "GridField.h"
#include "GridMapping.h"
#include "helper.h"
//#include "Pressure\pcgsolver\blas_win.h"
#include "Water/Particles.h"

#ifdef __APPLE__
#include "sparse_matrix.h"
#include "pcg_solver.h"
#elif defined _WIN32 || defined _WIN64
#include "Pressure\pcgsolver\sparse_matrix.h"
#include "Pressure\pcgsolver\pcg_solver.h"
#endif

class Fire{
protected:
    double computeDT(double currentTime);
	void advectLevelSet(double duration);

	void project(double duration);
    void project2D(double duration);
	double getAlpha(const int i, const int j, const int k, DirectionEnums d);
	double getDensity(const int i, const int j, const int k, DirectionEnums d);

	void computeW();
	void computeCellTypes();
	CellType getCellType(const int i, const int j, const int k) const;
	CellType getCellType(double w_x, double w_y,double w_z) const;

	void advect(double dt);
    void advect(double dt, GridField<int > &cellType);

	//void 
public:
	Fire(FirePresets *preset);
	~Fire();

    void runSimulation();

	void drawCenterGradients(Discretization *disc);
    void drawFaceVelocities();
	void drawCenterVelocities();
	void draw();
    void drawMAC();
    void drawSolid();
    void drawCellTypes();
    void drawDivergence();
    void drawScalar();
    void drawParticles();

	static CellType getCellType(double phi);
private:

    FirePresets *preset;
    LevelSet phi;

	//Pressure field
	GridField<double> *p;
	GridField<double> *rhs;
	std::vector<double> pVec;
	std::vector<double> rhsVec;

	// Sparse matrix A and solver
	SparseMatrix<double> *A;
	PCGSolver<double> *pcgSolver;
	double* resid_out;
	int iter_out;

	// Border conditions
	BorderCondition *_borderCondition;

    //Grid
    MACGrid u;
	// Levelset Velocities u + S*N
	GridField<Vector3> w;
    //Projektion
    Particles particles;
	GridField<int> celltype;
    //GridField<double> scalar;
};

#endif //FIRE_H
