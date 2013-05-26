#ifndef BLACKBODYRADIATION_H
#define BLACKBODYRADIATION_H
class Vector3;
class LevelSet;
template<class T> class GridField;

class BlackBodyRadiation
{
public:

	static double radiance(double lambda, double T);
	static Vector3 blackbodyToXYZ(double T);
	static Vector3 XYZtoLMS(const Vector3 &xyz);
	static Vector3 LMStoXYZ(const Vector3 &lms);
	static Vector3 XYZtoRGB(const Vector3 &xyz);
	
	static void draw(const GridField<double> &temperatureGrid, const LevelSet &phi); //Kan inte använda T->grid->xdim() av något okänt skäl, så skickar istället in grid
	static void drawLevelSet(const GridField<double> &temperatureGrid, const LevelSet &phi);
};

#endif