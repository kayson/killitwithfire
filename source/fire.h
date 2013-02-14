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
	CellType computeCellType(const int i, const int j, const int k) const;

	//void 
public:
	Fire(FirePresets *preset);
	~Fire();

    void runSumulation();

	void draw();

private:

    FirePresets *preset;
    LevelSet phi;

    //Grid
    VelocityField u;

	GridField<CellType> celltype;
};

#endif //FIRE_H
