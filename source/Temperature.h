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

    void AdvectTemperatureField(double dt, MACGrid m, LevelSet ls);

	void CalculateBuoyancyForceField();

	void draw();

	GridField<Vector3> *beyonce;


private:
	GridField<double> *grid; 
    double calculateTemperatureLoss(int i, int j, int k);
	void InitCell(int i, int j, int k, CellType type);
    void ResetCell(int i, int j, int k, CellType type);
	void drawBuoyancyForce();
};

#endif //TEMPERATURE_H
