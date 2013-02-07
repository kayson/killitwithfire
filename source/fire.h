#ifndef FIRE_H
#define FIRE_H

#include "presets/firePresets.h"
#include "VelocityField.h"
#include "levelset/LevelSet.h"

class Fire{
protected:
    double computeDT();
    void advect(double duration);
public:
	Fire(FirePresets *preset);
	~Fire();

    void runSumulation(double duration);

	void draw();

private:

    FirePresets *preset;
    LevelSet phi;
    //Grid
    VelocityField u;
};

#endif //FIRE_H
