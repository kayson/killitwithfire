/*
	Bas-klass för olika diskretiseringsmetoder. Virtuella metoder
	skall överlagras av sub-klasser
*/

#ifndef DISCRETIZATION_H
#define DISCRETIZATION_H
#include <iostream>
#include "Grid.h"
#include "datatype.h"
#include "VectorGrid.h"

class Discretization{
protected:
	VectorGrid *_w;
public:
	//Discretization();
	virtual ~Discretization(){};

	// Dessa skall och måste överlagras av subklasser
	virtual double calcDx(Grid &g, const int i, const int j, const int k)=0;
	virtual double calcDy(Grid &g, const int i, const int j, const int k)=0;
	virtual double calcDz(Grid &g, const int i, const int j, const int k)=0;

	virtual double calcD2x(Grid &g, const int i, const int j, const int k)=0;
	virtual double calcD2y(Grid &g, const int i, const int j, const int k)=0;
	virtual double calcD2z(Grid &g, const int i, const int j, const int k)=0;

	virtual double calcDyz(Grid &g, const int i, const int j, const int k)=0;
	virtual double calcDxz(Grid &g, const int i, const int j, const int k)=0;
	virtual double calcDxy(Grid &g, const int i, const int j, const int k)=0;
	virtual void setVectorGrid(VectorGrid *w){_w = w;};
};
#endif