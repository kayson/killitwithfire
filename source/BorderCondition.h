// Johan Noren
#ifndef BORDERCONDITION_H
#define BORDERCONDITION_H

class MACGrid;
template<class T> class GridField;

class BorderCondition{
public:
	bool checkBorder(GridField<double> &g, const int i, const int j, const int k) const;
	void enforceBorderCondition(MACGrid &v, GridField<double> &g, const int i, const int j, const int k) const;
private:
};

#endif