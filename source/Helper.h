#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include "Grid.h"
#include "datatype.h"

#ifdef __APPLE__
#include "armadillo.h"
#elif defined _WIN32 || defined _WIN64
#include "armadillo"
#endif

using namespace arma;

// Skicka in referens till grid samt vilken position
// man vill ha gradient eller divergens osv. av.
class Helper
{
public:
	Helper();
	~Helper();
	vec gradient(Grid &g, int i, int j, int k);
	data divergence(Grid &g, int i, int j, int k); //Bör vara Grid<Vector3> när vi har en vector3 klass
private:
	data calcDx(Grid &g, int i , int j, int k);
	data calcDy(Grid &g, int i , int j, int k);
	data calcDz(Grid &g, int i , int j, int k);

	data calcDxCentralDiff(Grid &g, int i , int j, int k);
	data calcDyCentralDiff(Grid &g, int i , int j, int k);
	data calcDzCentralDiff(Grid &g, int i , int j, int k);
};
#endif