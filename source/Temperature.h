#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "helper.h"

class Vector3;
template<class T> class GridField;
class MACGrid;
class LevelSet;



class Temperature{

public:
	Temperature(){};
	Temperature(GridField<double> *phi);
	~Temperature(){};

	GridField<double> GetTemperatureGrid();

    void AdvectTemperatureField(double dt, const MACGrid &m, const LevelSet &ls);

	void CalculateBuoyancyForceField(LevelSet &ls);
	double maxTemp();
	void draw();
	void drawBuoyancyForce();
	GridField<Vector3> *beyonce;

	GridField<double> *grid;
    GridField<double> *copy;
private:

    double calculateTemperatureLoss(int i, int j, int k);
	void InitCell(int i, int j, int k, CellType type);
    void ResetCell(int i, int j, int k, CellType type);
	
};

#endif //TEMPERATURE_H
