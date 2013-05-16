#ifndef BLACKBODYRADIATION_H
#define BLACKBODYRADIATION_H
class Vector3;
template<class T> class GridField;

class BlackBodyRadiation
{
public:

	static double radiance(double lambda, double T);
	static Vector3 blackbodyToXYZ(double T);
	static Vector3 XYZtoLMS(Vector3 xyz);
	static Vector3 LMStoXYZ(Vector3 lms);
	static Vector3 XYZtoRGB(Vector3 xyz);
	
	static void draw(const GridField<double> * const temperatureGrid); //Kan inte använda T->grid->xdim() av något okänt skäl, så skickar istället in grid
};

#endif