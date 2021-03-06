#ifndef LEVELSET_H
#define LEVELSET_H

class Vector3;
template<class T> class GridField;
class MACGrid;
class DetonationShockDynamics;

#include "helper.h"
#ifdef __APPLE__
#include "transform.hpp"
#elif defined __unix__
#include "glm.hpp"
#include "glm/gtx/transform.hpp"
#elif defined _WIN32 || defined _WIN64
#include "glm.hpp"
#include "glm/gtx/transform.hpp"
#endif
#include "meshtovolumeconverter.h"
class LevelSet
{
public:
	LevelSet();
	LevelSet(int xDim, int yDim, int zDim, double size);
	~LevelSet(){}

	void fillLevelSet(double (*implicitFunction)(int x, int y, int z));
	void fillLevelSetFromConverter(MeshToVolumeConverter &converter);
	void specifyRenderFunction(void (*renderFunction)());

	double getVolume() const;
	void drawNormals() const;
	void draw() const;
	void printDistanceField();
	double getCurvature(const int i, const int j, const int k) const;
	static double getCurvature(const int i, const int j, const int k, GridField<double> *grid);
	Vector3 getNormal(const int i, const int j, const int k) const;
	Vector3 getNormal(const double w_x, const double w_y, const double w_z) const;
	double getCurvature(const int i, const int j, const int k);
	Vector3 getFlameSpeed(const int i, const int j, const int k, MACGrid *vel) const;
	void updateNormals();
	void updateDSD(double dt, MACGrid *u);

	CellType getCellType(const int i, const int j, const int k) const;
	CellType getCellType(const double w_x, const double w_y, const double w_z) const;

	static CellType getCellType(const double phi);

	void reinitialize();

	GridField<double> *grid; //Signed distance field
	GridField<double> *gridCopy;
	GridField<Vector3> *normals;

    
    Vector3 getVelocity(MACGrid &g, int i, const int j, const int k);
    
private:
	void initLevelSet();

	DetonationShockDynamics *dsd;
	void (*render)();
};
#endif //LEVELSET_H
