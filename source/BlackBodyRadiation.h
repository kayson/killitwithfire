#ifndef BLACKBODYRADIATION_H
#define BLACKBODYRADIATION_H

#if defined __APPLE__
#include "glfw.h"
#elif defined _WIN32 || defined _WIN64 || __unix__
#include <GL/glfw.h>
#include <GL/freeglut.h>
#endif

#include <utility>

class Vector3;
class LevelSet;
template<class T> class GridField;
class SmokeDensity;

class BlackBodyRadiation
{
public:
	BlackBodyRadiation();
	BlackBodyRadiation(const int XPIXELS, const int YPIXELS, const GridField<double> &temperatureGrid);
	BlackBodyRadiation(const BlackBodyRadiation &);
    BlackBodyRadiation & operator= (const BlackBodyRadiation & other);

	~BlackBodyRadiation();

	static double radiance(double lambda, double T);
	static Vector3 blackbodyToXYZ(double T);
	static Vector3 XYZtoLMS(const Vector3 &xyz);
	static Vector3 LMStoXYZ(const Vector3 &lms);
	static Vector3 XYZtoRGB(const Vector3 &xyz);
	
	void draw(const GridField<double> &temperatureGrid, const LevelSet &phi, const SmokeDensity &smoke); 
	void drawColor(const GridField<double> &temperatureGrid, const LevelSet &phi, const SmokeDensity &smoke); 
	void drawFireBlueCore(const GridField<double> &temperatureGrid, const LevelSet &phi, const SmokeDensity &smoke); 
	void drawBlueCore(const GridField<double> &temperatureGrid, const LevelSet &phi, const SmokeDensity &smoke); 

private:
	void allocate();
	void deallocate();

	GLfloat *image;
	GLuint textureID;

	double dx, dy, dz;
	double wds; // ray caster sample step
	double oa, os, ot, C;

	double du, dv;// def [-1, 1] screen space coordinates

	int xsize, ysize, zsize;

	int gridx, gridy, gridz;

	double *Le;
	double *L;
	
	double *LeMean;
	double *xm;
	double *ym;
	double *zm;

	double angle;

	double funVar2;
	int funVar;
};

#endif