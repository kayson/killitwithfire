

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
    void advectVelocityField(double duration);
	void advectLevelSet(double duration);

	void computeCellTypes();
	CellType getCellType(const int i, const int j, const int k);

	//void 
public:
	Fire(FirePresets *preset);
	~Fire();

    void runSumulation();

	void drawCenterVelocities();
	void draw();

private:

    FirePresets *preset;
    LevelSet phi;

    //Grid
    VelocityField u;

	GridField<int> celltype;
};

#endif //FIRE_H
