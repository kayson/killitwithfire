#include "BlackBodyRadiation.h"
#include <stdio.h>
#include <cmath>

double C_1 = 3.7418e-16;
double C_2 = 1.4388e-2;

double BlackBodyRadiation::radiance(double lambda, double T)
{
	// Equation 22, Nguyen02
	lambda *= 1e-9;
	return (2*C_1) / (pow(lambda,5) * (exp(C_2/(lambda*T)) - 1) );
}

float BlackBodyRadiation::red(double T)
{
	double radiance_tot = 0.;
	#pragma omp parallel for
	for(float i = 430; i < 700; i+=5) 
		radiance_tot += radiance(i,T);
	
	return 1 - exp(-(float)radiance_tot);
}

float BlackBodyRadiation::green(double T)
{
	double radiance_tot = 0.;
	#pragma omp parallel for
	for(float i = 400; i < 650; i+=5) 
		radiance_tot += radiance(i,T);

	return 1 - exp(-(float)radiance_tot);
}

float BlackBodyRadiation::blue(double T)
{
	double radiance_tot = 0.;
	#pragma omp parallel for
	for(float i = 400; i < 500; i+=5) 
		radiance_tot += radiance(i,T);

	return 1 - exp(-(float)radiance_tot);
}

