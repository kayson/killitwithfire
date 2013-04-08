
#include <climits>
#include <iostream>
#include <cmath>


#include "GridField.hpp"
#include "presets/firePresets.h"
#include "Gradient.h"

#include "Reinitialize.h"

#include "integrateEuler.h"


void reinitialize::Godunov(GridField<double> &g, int i, int j, int k, double a, double &ddx2, double &ddy2, double &ddz2)
{
	double ddxm = FirePresets::upwindDisc->calcDxm(g, i, j, k);
	double ddxp = FirePresets::upwindDisc->calcDxp(g, i, j, k);
	double ddym = FirePresets::upwindDisc->calcDym(g, i, j, k);
	double ddyp = FirePresets::upwindDisc->calcDyp(g, i, j, k);
	double ddzm = FirePresets::upwindDisc->calcDzm(g, i, j, k);
	double ddzp = FirePresets::upwindDisc->calcDzp(g, i, j, k);
	if (a > 0.0) {
		ddx2 = std::max( std::pow(std::max(ddxm,0.0),2.0), std::pow(std::min(ddxp,0.0),2.0) );
		ddy2 = std::max( std::pow(std::max(ddym,0.0),2.0), std::pow(std::min(ddyp,0.0),2.0) );
		ddz2 = std::max( std::pow(std::max(ddzm,0.0),2.0), std::pow(std::min(ddzp,0.0),2.0) );
	}
	else {
		ddx2 = std::max( std::pow(std::min(ddxm,0.0),2.0), std::pow(std::max(ddxp,0.0),2.0) );
		ddy2 = std::max( std::pow(std::min(ddym,0.0),2.0), std::pow(std::max(ddyp,0.0),2.0) );
		ddz2 = std::max( std::pow(std::min(ddzm,0.0),2.0), std::pow(std::max(ddzp,0.0),2.0) );
	}

}
double reinitialize::CalcMaxGradient(GridField<double> &g)
{
	Gradient *gradient = new Gradient();
	double max = DBL_MIN;
	for(int i = 0; i < g.xdim(); i++)
	{
		for(int j = 0; j < g.ydim(); j++)
		{
			for(int k = 0; k < g.zdim(); k++)
			{
				if(g.valueAtIndex(i,j,k) < 0){
					double v = (gradient->getGradient(g, i, j, k, *FirePresets::upwindDisc)).norm();
					if(v > max)
						max = v;
				}
			}
		}
	}
	return max;
}

double reinitialize::CalcMeanGradient(GridField<double> &g)
{
	Gradient *gradient = new Gradient();
	double meanGradient = 0;
	for(int i = 0; i < g.xdim(); i++)
	{
		for(int j = 0; j < g.ydim(); j++)
		{
			for(int k = 0; k < g.zdim(); k++)
			{
				meanGradient += (gradient->getGradient(g, i, j, k, *FirePresets::upwindDisc)).norm();
			}
		}
	}
	return meanGradient / (g.xdim() * g.ydim() * g.zdim());
}

double reinitialize::Evaluate(GridField<double> &g, int i, int j, int k)
{
	if(g.valueAtIndex(i,j,k) < 0)
	{
		//Räkna ut sign-funktion (mha central diff)
		double dx = FirePresets::dx;
		double ddx = FirePresets::centralDisc->calcDx(g, i, j, k);
		double ddy = FirePresets::centralDisc->calcDy(g, i, j, k);
		double ddz = 0.0;
		if(g.zdim() != 1)
			ddz = FirePresets::centralDisc->calcDz(g, i, j, k);

		double normalGradient = ddx * ddx + ddy * ddy + ddz * ddz;
		double val = g.valueAtIndex(i,j,k);

		double sign = val / sqrt(val * val + normalGradient * dx * dx);

		//Räkna ut upwind differences med Godunov
		double ddx2, ddy2, ddz2;
		Godunov(g, i, j, k, sign, ddx2, ddy2, ddz2);
		return sign * (1.0 - std::sqrt(ddx2 + ddy2 + ddz2));
	}
	else
		return 0;
		
}

void reinitialize::reinitializeGrid(GridField<double> **g, GridField<double> **gridCopy)
{
	double time = FirePresets::dt;
	double dt = 0.5 * FirePresets::dx;
		
	for(double elapsed = 0; elapsed < time;)
	{
		if(dt > time)
			dt = time - elapsed;
		elapsed += dt;

		//Integrate
		double mean = CalcMeanGradient(**g);

		IntegrateEuler *e = new IntegrateEuler();
		e->calculateIntegral(g, gridCopy, dt, Evaluate);		
	}
}

