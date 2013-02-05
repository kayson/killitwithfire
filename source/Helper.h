#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include "Grid.h"
#include "datatype.h"
#include "armadillo"

using namespace arma;

// Skicka in referens till grid samt vilken position
// man vill ha gradient eller divergens osv. av.
class Helper
{
public:
	Helper();
	~Helper();
	vec gradient(Grid<data> &g, int i, int j, int k);
	data divergence(Grid<data> &g, int i, int j, int k); //Bör vara Grid<Vector3<data>> när vi har en vector3<data> klass
private:
	data calcDx(Grid<data> &g, int i , int j, int k);
	data calcDy(Grid<data> &g, int i , int j, int k);
	data calcDz(Grid<data> &g, int i , int j, int k);

	data calcDxCentralDiff(Grid<data> &g, int i , int j, int k);
	data calcDyCentralDiff(Grid<data> &g, int i , int j, int k);
	data calcDzCentralDiff(Grid<data> &g, int i , int j, int k);
};
#endif