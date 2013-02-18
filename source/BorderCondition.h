// Johan Noren
#ifndef BORDERCONDITION_H
#define BORDERCONDITION_H
#include "GridField.hpp"
#include "MACGrid.h"
#include "Vector3.h"
class BorderCondition{
public:
	bool checkBorder(GridField<double> &g, const int i, const int j, const int k) const;
	void enforceBorderCondition(MACGrid &v, GridField<double> &g, const int i, const int j, const int k) const;
private:
};

#endif