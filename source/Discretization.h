/*
	Bas-klass f�r olika diskretiseringsmetoder. Virtuella metoder
	skall �verlagras av sub-klasser
*/

#ifndef DISCRETIZATION_H
#define DISCRETIZATION_H
#include <iostream>
#include "Grid.h"
#include "datatype.h"

class Discretization{
public:
	//Discretization();
	virtual ~Discretization(){};

	// Dessa skall och m�ste �verlagras av subklasser
	virtual data calcDx(const Grid &g, const int i, const int j, const int k)=0;
	virtual data calcDy(const Grid &g, const int i, const int j, const int k)=0;
	virtual data calcDz(const Grid &g, const int i, const int j, const int k)=0;

	virtual data calcD2x(const Grid &g, const int i, const int j, const int k)=0;
	virtual data calcD2y(const Grid &g, const int i, const int j, const int k)=0;
	virtual data calcD2z(const Grid &g, const int i, const int j, const int k)=0;

	virtual data calcDyz(const Grid &g, const int i, const int j, const int k)=0;
	virtual data calcDxz(const Grid &g, const int i, const int j, const int k)=0;
	virtual data calcDxy(const Grid &g, const int i, const int j, const int k)=0;

};
#endif