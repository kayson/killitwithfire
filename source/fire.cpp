
#include <iomanip>
#include "fire.h"

#include "advect/AdvectLevelSet.h"
#include "GridField.hpp"
#include "Gradient.h"
#include "GridFieldFileManager.h"

#ifdef __APPLE__
#include "glfw.h"
#include "pcg_solver.h"
#include <Accelerate/Accelerate.h>
#include "transform.hpp"
#elif defined _WIN32 || defined _WIN64
#include <GL/glfw.h>

#include "Vorticity.h"

#endif

Fire::Fire(FirePresets *pre):phi(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z,preset->GRID_SIZE), w(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z,preset->GRID_SIZE),celltype(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z),u(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z, preset->GRID_SIZE)
{
	//Presets
	preset = pre;

	phi.fillLevelSet(preset->implicitFunction);
	//2D grid
	u = MACGrid::createRandom2D(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_SIZE);
	//Advect

	p = new GridField<double>(phi.grid->xdim(), phi.grid->ydim(), phi.grid->zdim());
	rhs = new GridField<double>(phi.grid->xdim(), phi.grid->ydim(), phi.grid->zdim());
	pVec.reserve( phi.grid->xdim() * phi.grid->ydim() * phi.grid->zdim() );
	rhsVec.reserve( phi.grid->xdim() * phi.grid->ydim() * phi.grid->zdim() );

	const int matDim = phi.grid->xdim()*phi.grid->ydim()*phi.grid->zdim()*phi.grid->xdim()*phi.grid->ydim()*phi.grid->zdim();

	//A = new SparseMatrix<double>(matDim, 7); // Total matrix, antal icke-zeros per rad
	pcgSolver = new PCGSolver<double>();
	resid_out = new double();
	iter_out = 100;

	_borderCondition = new BorderCondition();

	preset->upwindDisc->setVelocityField(w);

	T = new Temperature(phi.grid);

	vorticityForces = new GridField<Vector3>(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z);
}

double Fire::computeDT(double currentTime){
	double smallStep;

	//Bridson s. 35
	double dx = preset->dx;
	double alpha = preset->CFL_NUMBER;
	double c = w.getMax().norm();
	if(c != 0)
		smallStep = alpha * dx / c;
	else
		smallStep = dx;

	//Fixa overshoot

	if(smallStep > preset->dt - currentTime)
	{
		smallStep = preset->dt - currentTime;
	}
	return smallStep;
}

void Fire::advectLevelSet(double duration)
{
	computeW();
	preset->advection->advect(w, phi, duration);
}

void Fire::advectTemperature(double dt)
{
    T->AdvectTemperatureField(dt, u, phi);
}

double Fire::getAlpha(const int i, const int j, const int k, DirectionEnums d)
{
	// sid. 104 (Bridson)
	double temp = 0;

	if(d == RIGHT)
		temp = phi.grid->valueAtIndex(i+1,j,k);
	if(d == LEFT)
		temp = phi.grid->valueAtIndex(i-1,j,k);
	if(d == UP)
		temp = phi.grid->valueAtIndex(i,j+1,k);
	if(d == DOWN)
		temp = phi.grid->valueAtIndex(i,j-1,k);
	if(d == FORWARD)
		temp = phi.grid->valueAtIndex(i,j,k+1);
	if(d == BACKWARD)
		temp = phi.grid->valueAtIndex(i,j,k-1);

	if(phi.grid->valueAtIndex(i,j,k) <= 0 && temp <= 0)
		return 1;
	else if(phi.grid->valueAtIndex(i,j,k) <= 0 && temp > 0)
		return (phi.grid->valueAtIndex(i,j,k) / 
		(phi.grid->valueAtIndex(i,j,k) - temp));
	else if(phi.grid->valueAtIndex(i,j,k) > 0 && temp <= 0)
		return 1 - (phi.grid->valueAtIndex(i,j,k) / 
		(phi.grid->valueAtIndex(i,j,k) - temp));
	else if(phi.grid->valueAtIndex(i,j,k) > 0 && temp > 0)
		return 0;
}

double Fire::getDensity(const int i, const int j, const int k, DirectionEnums d)
{
	// sid. 104 (Bridson)
	double alpha = getAlpha(i,j,k,d);
	CellType temp;

	if(d == LEFT)
		temp = getCellType(i-1,j,k);
	if(d == RIGHT)
		temp = getCellType(i+1,j,k);
	if(d == DOWN)
		temp = getCellType(i,j-1,k);
	if(d == UP)
		temp = getCellType(i,j+1,k);
	if(d == BACKWARD)
		temp = getCellType(i,j,k-1);
	if(d == FORWARD)
		temp = getCellType(i,j,k+1);

	if(getCellType(i,j,k) == BLUECORE && temp == BLUECORE)
		return preset->rhof;
	else if(getCellType(i,j,k) == BLUECORE && temp == IGNITED)
		return alpha * preset->rhof + ( 1 - alpha ) * preset->rhoh;
	else if(getCellType(i,j,k) == IGNITED && temp == BLUECORE)
		return alpha * preset->rhoh + ( 1 - alpha ) * preset->rhof;
	else if(getCellType(i,j,k) == IGNITED && temp == IGNITED)
		return preset->rhoh;

}

void Fire::computeCellTypes()
{
	for(GridFieldIterator<int> it = celltype.iterator(); !it.done(); it.next())
	{
		int i, j, k;
		it.index(i, j, k);

		celltype.setValueAtIndex(getCellType(i,j,k), i, j, k);

	}
}	

CellType Fire::getCellType(const int i, const int j, const int k) const
{
	if(false) //Check if is solid
		return SOLID;
	else if(phi.grid->valueAtIndex(i,j,k) > 0.0)
		return BLUECORE;
	else 
		return IGNITED;
}

CellType Fire::getCellType(double w_x, double w_y,double w_z) const
{
	return getCellType(phi.grid->valueAtWorld(w_x, w_y, w_z));
}

CellType Fire::getCellType(double phi)
{
	if(false) //Check if is solid
		return SOLID;
	else if(phi > 0.0)
		return BLUECORE;
	else 
		return IGNITED;
}

void Fire::runSimulation(){

	 //Advektera levelset
    for(double currentTime = 0; currentTime < preset->dt;)
	{
		double dt = computeDT(currentTime);

		//Advektera hastighestsfältet
		//advectLevelSet(preset->dt);

		currentTime += dt;
	}


	static int counter = 0;
    if (counter % 1 == 0) {
		for(int i = -7; i < 7; i++)
		{
			phi.grid->addValueAtIndex(1,preset->GRID_DIM_X/2+i,0,0);
			u.addValueAtFace(3,preset->GRID_DIM_X/2+i,0,0,UP);
		}
    }
    counter++;

	//Beräkna om vad för typ voxlarna är
	computeCellTypes(); 

	//u.advect(preset->dt);
	preset->advectVelocities->advect(u, phi, preset->dt);


    Vector3 force = Vector3(0.0, 0.05, 0.0);
    u.addForce(force, preset->dt);
	
	// Vorticity confinement forces
	Vorticity::addVorticity(u, *vorticityForces, 2.5, FirePresets::dx, 
		phi.grid->xdim(), phi.grid->ydim(), phi.grid->zdim());

	u.addForceGrid(*vorticityForces, preset->dt); // Add vorticity forces to velocity field

	advectLevelSet(preset->dt);

    //Externa krafter
		//preset->externalForce->addForce(grid);
    
    //Project
	//project(preset->dt);
	//project2D(preset->dt);

	//Advektera temperatur
	advectTemperature(preset->dt);


	//Fixa signed distance field
	phi.reinitialize();

}

void Fire::drawVorticities(){
	glColor3d(1.0,1.0,1.0);
	for( GridFieldIterator<Vector3> iter = vorticityForces->iterator(); !iter.done(); iter.next() ){
		int i,j,k;
		iter.index(i,j,k);
		double x,y,z;
		vorticityForces->indexToWorld(i,j,k,x,y,z);
		Vector3 val = iter.value();
		/*std::cout << "ijk: "<< i << " " << j << " " << k <<std::endl;
		std::cout << "xyz: "<< x << " " << y << " " << z <<std::endl<<std::endl;*/

		glBegin(GL_LINE_STRIP);
		glVertex3d(x,y,0);
		glVertex3d(x+val.x, y+val.y,0);
		glEnd();
	}
}

void Fire::drawMAC(){

	glColor3f(1,1,0);
	glBegin(GL_POINTS);
	for (GridMappingIterator iter = u.iterator(); !iter.done(); iter.next()) {
		int i,j,k;
		iter.index(i, j, k);
		double x,y,z;
		u.indexToWorld(i, j, k, x, y, z);

		glVertex3d(x, y, 0);
	}
	glEnd();

}

void Fire::drawSolid(){

	double dx = celltype.dx();
	double dy = celltype.dy();
	//double dz = celltype.mapping.dx();
	glBegin(GL_QUADS);
	for (GridFieldIterator<int> iter = celltype.iterator(); !iter.done(); iter.next()) {
		double x,y,z;
		int i,j,k;
		iter.index(i, j, k);

		celltype.indexToWorld(i, j, k, x, y, z);
		int val = iter.value();

		if (val == SOLID){
			glColor3d(0.1, 0.1, 0.1);
			glVertex3d(x-dx*0.5, y-dy*0.5, 0);
			glVertex3d(x+dx*0.5, y-dy*0.5, 0);
			glVertex3d(x+dx*0.5, y+dy*0.5, 0);
			glVertex3d(x-dx*0.5, y+dy*0.5, 0);
		}else if (val == BLUECORE){
			glColor3f(0, 0, 1);
			glVertex3d(x-dx*0.5, y-dy*0.5, 0);
			glVertex3d(x+dx*0.5, y-dy*0.5, 0);
			glVertex3d(x+dx*0.5, y+dy*0.5, 0);
			glVertex3d(x-dx*0.5, y+dy*0.5, 0);
		}else if (val == AIR){
			//None dude
		}


	}

	glEnd();
}

void Fire::drawFaceVelocities(){
	glColor3f(0,1,0);
	for (GridFieldIterator<double> iter = u._u->iterator(); !iter.done(); iter.next()) {
		int i,j,k;
		iter.index(i, j, k);
		double x,y,z;
		u._u->indexToWorld(i, j, k, x, y, z);
		double val = iter.value();

		glBegin(GL_LINE_STRIP);
		glVertex3d(x, y, 0);
		glVertex3d(x+val, y, 0);
		glEnd();
	}

	glColor3f(1,0,0);
	for (GridFieldIterator<double> iter = u._v->iterator(); !iter.done(); iter.next()) {
		int i,j,k;
		iter.index(i, j, k);
		double x,y,z;
		u._v->indexToWorld(i, j, k, x, y, z);
		double val = iter.value();
		glBegin(GL_LINE_STRIP);
		glVertex3d(x, y, 0);
		glVertex3d(x, y+val, 0);
		glEnd();
	}

}

void Fire::drawCenterGradients(Discretization *disc)
{
	for (GridMappingIterator iter = u.iterator(); !iter.done(); iter.next()) {
		int i,j,k;
		iter.index(i, j, k);
		double x,y,z;
		u.indexToWorld(i, j, k, x, y, z);
		GridField<double> *g = phi.grid;
		Vector3 v = Gradient::getGradient(*g, i, j, k, *disc) * 0.2;
		//std::cout << v.norm() << std::endl;
		/*glColor3f(0,1,0);
		glBegin(GL_LINES);
		glVertex3f(1, 1, 1);
		glVertex3f(dx*i + 1, y + 1, z + 1);
		glEnd();

		glColor3f(0,0,0);
		glBegin(GL_POINTS);
		glVertex3f(x + v.x, y + v.y, z + v.z);
		glEnd();
		*/

		//x = i; y = j; z = k;

		glColor3d(1.0,1.0,0.0);
		glBegin(GL_LINE_STRIP);
		glVertex3d(x, y, 0);
		glVertex3d(x + v.x, y+v.y , 0);
		glEnd();

		/*
		glColor3f(1,0,0);
		glBegin(GL_POINTS);
		glVertex3f(x, y, 0);
		glEnd();
		*/

	}
	/*
	for(int i = 0; i < phi.grid.xdim(); i += 5)
	>>>>>>> Tagit bort vectorgrid
	{
	for(int j = 0; j < phi.grid->ydim(); j += 5)
	{
	for(int k = 0; k < phi.grid->zdim(); k += 5)
	{
	float x0 = FirePresets::dx*((double)(i) + 0.5);
	float y0 = FirePresets::dx*((double)(j) + 0.5);
	float z0 = FirePresets::dx*((double)(k) + 0.5);

	Vector3 v = u.velocityAtIndex(Vector3(i, j, k))*FirePresets::dx;
	glColor3f(0,1,0);
	glBegin(GL_LINES);
	glVertex3f(x0, y0, z0);
	glVertex3f(x0 + v.x, y0 + v.y, z0 + v.z);
	glEnd();

	glColor3f(0,0,0);
	glBegin(GL_POINTS);
	glVertex3f(x0 + v.x, y0 + v.y, z0 + v.z);
	glEnd();
	}
	}
	}*/

}

void Fire::drawCenterVelocities(){
    for (GridMappingIterator iter = u.iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        u.indexToWorld(i, j, k, x, y, z);
    
        Vector3 v = u.velocityAtWorld(Vector3(x,y,z));//*FirePresets::dx;
        glColor3d(1.0,1.0,0.0);
        glBegin(GL_LINE_STRIP);
        glVertex3d(x, y, 0);
        glVertex3d(x + v.x, y +v.y , 0);
        glEnd();
    }
}

void Fire::computeW()
{
	for(GridFieldIterator<Vector3> it = w.iterator(); !it.done(); it.next())
	{
		int i, j, k;
		it.index(i,j,k);
		Vector3 v = u.velocityAtCenter(i, j, k) + phi.getNormal(i, j, k)*FirePresets::S;
		w.setValueAtIndex(v, it.index());
	}
}

void Fire::draw()
{
	//phi.draw();
    T->draw();

	//drawVorticities();
    //u.draw();
	//drawCenterVelocities();
    //drawCenterGradients(FirePresets::upwindDisc);
    //drawFaceVelocities();
    //drawMAC();
    //drawSolid();

}

Fire::~Fire(){

    delete preset;
	//delete pcgSolver;
	delete A;
	delete rhs;
	delete p;
	delete _borderCondition;
}
/*
void Fire::advect(double dt){

for (GridFieldIterator<double> iter = u._u->iterator(); !iter.done(); iter.next()) {
int i,j,k;
iter.index(i, j, k);
double x,y,z;
u._u->indexToWorld(i, j, k, x, y, z);
double val = preset->advectVelocities->advect(dt, u, *u._u, i, j, k);
u.buffer()->_u->setValueAtIndex(val, i, j, k);

}

for (GridFieldIterator<double> iter = u._v->iterator(); !iter.done(); iter.next()) {
int i,j,k;
iter.index(i, j, k);
double x,y,z;
u._v->indexToWorld(i, j, k, x, y, z);
double val = preset->advectVelocities->advect(dt, u, *u._v, i, j, k);
u.buffer()->_v->setValueAtIndex(val, i, j, k);

}

for (GridFieldIterator<double> iter = u._w->iterator(); !iter.done(); iter.next()) {
int i,j,k;
iter.index(i, j, k);
double x,y,z;
u._w->indexToWorld(i, j, k, x, y, z);
double val = preset->advectVelocities->advect(dt, u, *u._w, i, j, k);
u.buffer()->_w->setValueAtIndex(val, i, j, k);
}

u.swapBuffer();
}

void Fire::advect(double dt, GridField<int > &cellType){

for (GridFieldIterator<double> iter = u._u->iterator(); !iter.done(); iter.next()) {
int i,j,k;
iter.index(i, j, k);
double x,y,z;
u._u->indexToWorld(i, j, k, x, y, z);
double val;


if (cellType.valueAtWorld(x, y, z) == BLUECORE) 
{
val = preset->advectVelocities->advect(dt, u, *u._u, i, j, k);
}
else
{
val = 0;// _u->valueAtIndex(iter.index());
}

u.buffer()->_u->setValueAtIndex(val, i, j, k);

}

for (GridFieldIterator<double> iter = u._v->iterator(); !iter.done(); iter.next()) {
int i,j,k;
iter.index(i, j, k);
double x,y,z;
u._v->indexToWorld(i, j, k, x, y, z);
double val;
if (cellType.valueAtWorld(x, y, z) == BLUECORE) {
val = preset->advectVelocities->advect(dt, u, *u._v, i, j, k);
}else{
val = 0;//_v->valueAtIndex(iter.index());
}

u.buffer()->_v->setValueAtIndex(val, i, j, k);

}

for (GridFieldIterator<double> iter = u._w->iterator(); !iter.done(); iter.next()) {
int i,j,k;
iter.index(i, j, k);
double x,y,z;
u._w->indexToWorld(i, j, k, x, y, z);
double val;
if (cellType.valueAtWorld(x, y, z) == BLUECORE) {
val = preset->advectVelocities->advect(dt, u, *u._w, i, j, k);
}else{
val = 0;//_w->valueAtIndex(iter.index());
}

u.buffer()->_w->setValueAtIndex(val, i, j, k);

}

u.swapBuffer();
}*/