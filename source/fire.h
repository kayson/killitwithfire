

#ifndef FIRE_H
#define FIRE_H


#include "presets/firePresets.h"

#include "VelocityField.h"
#include "levelset/LevelSet.h"
#include "GridField.h"
#include "helper.h"



class Fire{
protected:
    double computeDT(double currentTime);
	void advectLevelSet(double duration);
	void project(double duration);
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

    //Grid
    MACGrid u;

	GridField<int> celltype;
};

#endif //FIRE_H
