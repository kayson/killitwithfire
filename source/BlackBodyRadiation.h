#ifndef BLACKBODYRADIATION_H
#define BLACKBODYRADIATION_H
class Vector3;

class BlackBodyRadiation
{
public:

	static double radiance(double lambda, double T);
	static Vector3 blackbodyToXYZ(double T);
	static Vector3 XYZtoLMS(Vector3 xyz);
	static Vector3 LMStoXYZ(Vector3 lms);
	static Vector3 XYZtoRGB(Vector3 xyz);
	

};

#endif