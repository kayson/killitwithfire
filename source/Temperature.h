#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "Vector3.h"
#include "GridField.h"
#include "helper.h"
#include "MACGrid.h"

class Temperature{

public:
	Temperature(){};
	Temperature(GridField<double> *phi);
	~Temperature(){};

	void SetToMax(int i, int j, int k);
	void SetToAir(int i, int j, int k);
	void SetToIgnite(int i, int j, int k);
	GridField<double> GetTemperatureGrid();
	void draw();

private:
	GridField<double> *grid; 

	void InitCell(int i, int j, int k, CellType type);
};

#endif //TEMPERATURE_H