#include "Helper.h"

Helper::Helper(){}

Helper::~Helper(){}

vec Helper::gradient(Grid &g, int i, int j, int k)
{
	std::cout << "gradient(..) called\n";
	vec v;
	return v;
}

float Helper::divergence(Grid &g, int i, int j, int k)
{
	std::cout << "divergence(..) called\n";
	return 0.f;
}