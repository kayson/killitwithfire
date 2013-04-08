#include "BlackBodyRadiation.h"
#include <stdio.h>
#include <cmath>

double C_1 = 3.7418 * pow(10.0, -16);
double C_2 = 1.4388 * pow(10.0 , -2);
double scale = 200;

double BlackBodyRadiation::radiance(double lambda, double T)
{
	// Equation 22, Nguyen02
	
	return (2*C_1) / (pow(lambda,5) * (exp(C_2/(lambda*T)) - 1) );
}

float BlackBodyRadiation::red(double T)
{
	double radiance_tot=0.;
	#pragma omp parallel for
	for(float i = 0.43; i < 0.7; i+=0.01) 
		radiance_tot += radiance(i,T)/0.00001;

	
	return 1 - exp(-(float)radiance_tot * scale);
}

float BlackBodyRadiation::green(double T)
{
	double radiance_tot=0.;
	#pragma omp parallel for
	for(float i = 0.4; i < 0.65; i+=0.01) 
		radiance_tot += radiance(i,T)/0.00001;

	return 1 - exp(-(float)radiance_tot * scale);
}

float BlackBodyRadiation::blue(double T)
{
	double radiance_tot=0.;
	#pragma omp parallel for
	for(float i = 0.4; i < 0.5; i+=0.01) 
		radiance_tot += radiance(i,T)/0.00001;

	return 1 - exp(-(float)radiance_tot * scale);
}

