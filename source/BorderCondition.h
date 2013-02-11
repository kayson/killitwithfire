#ifndef BORDERCONDITION_H
#define BORDERCONDITION_H
#include "Grid.h"
#include "VelocityField.h"
#include "Vector3.h"
class BorderCondition{
public:
	bool checkBorder(Grid &g, const int i, const int j, const int k);
	void enforceBorderCondition(VelocityField &v, Grid &g, const int i, const int j, const int k);
private:
};

#endif