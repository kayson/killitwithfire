#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "Vector3.h"
#include "GridField.h"
#include "helper.h"
#include "MACGrid.h"
#include "LevelSet.h"

class Temperature{

public:
	Temperature(){};
	Temperature(GridField<double> *phi);
	~Temperature(){};

	void SetToMax(int i, int j, int k);
	void SetToAir(int i, int j, int k);
	void SetToIgnite(int i, int j, int k);
	GridField<double> GetTemperatureGrid();

    void AdvectTemperatureField(double dt, MACGrid m, LevelSet ls);

	void draw();

private:
	GridField<double> *grid; 
    GridField<double> *gridCopy;
    double calculateTemperatureLoss(int i, int j, int k);
	void InitCell(int i, int j, int k, CellType type);
    void ResetCell(int i, int j, int k, CellType type);
};

#endif //TEMPERATURE_H