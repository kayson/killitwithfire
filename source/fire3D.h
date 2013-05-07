

#ifndef FIRE3D_H
#define FIRE3D_H

#include "presets/firePresets.h"

#include "levelset/LevelSet.h"

#include "GridField.h"
#include "GridMapping.h"
#include "helper.h"
#include "Particles.h"

//#include "Pressure\pcgsolver\blas_win.h"
#include "Water/Particles.h"
#include "Discretization.h"
#include "BorderCondition.h"
#include "GhostMAC.h"

#ifdef __APPLE__
#include "sparse_matrix.h"
#include "pcg_solver.h"
#include "Projection3D.h"
#elif defined _WIN32 || defined _WIN64
#include "Pressure\pcgsolver\sparse_matrix.h"
#include "Pressure\pcgsolver\pcg_solver.h"
#include "Pressure\Projection3D.h"
#endif
#include "GhostMAC.h"

#include "Temperature.h"

class FirePresets;
template<class T> class GridField;
class GridMapping;
class Fire3D{
protected:
    void setSolids();
    void computeGhostValues();
    double computeDT(double currentTime);
	void advectLevelSet(double duration);
	void advectTemperature(double duration);
    
	void project(double duration);
    void project2D(double duration);
	double getAlpha(const int i, const int j, const int k, DirectionEnums d);
	double getDensity(const int i, const int j, const int k, DirectionEnums d);
    
	void computeW();
	CellType getCellType(const int i, const int j, const int k) const;
	CellType getCellType(double w_x, double w_y,double w_z) const;
	//void
public:
	Fire3D(FirePresets *preset);
	~Fire3D();
    
	double computeFuelVolume();
    
	void addFuelToLevelSet(int x0, int y0, int z0, double radius);
    
    void runSimulation();
    
	void drawCenterGradients(Discretization *disc);
    void drawFaceVelocities();
    void drawFaceVelocities(MACGrid &grid) const;
	void drawCenterVelocities();
    void drawNormals() const;
	void draw();
    void drawMAC(MACGrid &grid);
    void drawMAC(MACGrid &grid, CellType cellType, double r,double g,double b);
    void drawBoundary(const GridMapping &g) const;
    template<class T2> void drawVoxels(const GridField<T2> &g) const;
    void drawGrid(const GridField<bool> &g) const;
    void drawSolid();
    void drawCellTypes();
    void drawDivergence();
    void drawScalar();
    void drawParticles();
	void drawVorticities();
    
    void enforceBorderCondition(); //NO PANIC! JUST FOR TESTING.
    
	static CellType getCellType(double phi);
private:
    Particles particles;
    FirePresets *preset;
    LevelSet phi;
    
	Temperature *T;
    
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
    MACGrid u_fuel, u_burnt;
    GridField<bool> solids;
    
    GhostMAC ghost;
	// Levelset Velocities u + S*N
	GridField<Vector3> w;
	
    //Projektion
    Projection3D projection;
    
    //GridField<double> scalar;
    
	//Vorticity confinement forces
	GridField<Vector3> *vorticityForces;
};

#endif //FIRE_H
