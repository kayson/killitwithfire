#ifndef BLACKBODYRADIATION_H
#define BLACKBODYRADIATION_H

class BlackBodyRadiation
{
public:
	static double radiance(double lambda, double T);
	static float red(double T);
	static float blue(double T);
	static float green(double T);
};

#endif