
#include <iomanip>
#include "fire3D.h"

#include "advect/AdvectLevelSet.h"
#include "GridField.hpp"
#include "Gradient.h"
#include "GridFieldFileManager.h"
#include "GridMapping.h"
#include "presets/firePresets.h"
#include "MACAdvect.h"
#include "Input.h"
#include "JumpCondition.h"
#include "BlackBodyRadiation.h"
#include "SmokeDensity.h"

//#include "meshtovolumeconverter.h"

#if defined __APPLE__
#include "glfw.h"
#include "pcg_solver.h"
#include <Accelerate/Accelerate.h>
#include "transform.hpp"
#elif defined __unix__
#include "GL/glfw.h"

#elif defined _WIN32 || defined _WIN64
#include <GL/glfw.h>
#endif
#include "Vorticity.h"

#include "ClosestValueExtrapolation.h"
#include "ConstantValueExtrapolation.h"

Fire3D::Fire3D(FirePresets *pre):
	phi(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z,preset->GRID_SIZE), 
	w(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z,preset->GRID_SIZE, new ClosestValueExtrapolation<Vector3>()),
	u(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z, preset->GRID_SIZE), 
	ghost(&phi, FirePresets::GRID_SIZE,true),projection(&ghost,&phi), 
	u_fuel(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z, preset->GRID_SIZE),
	u_burnt(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z, preset->GRID_SIZE),
	solids(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z, preset->GRID_SIZE, new ClosestValueExtrapolation<bool>()),
	smoke(preset->GRID_DIM_X*pre->TEMPERATUR_MULT, preset->GRID_DIM_Y*pre->TEMPERATUR_MULT, preset->GRID_DIM_Z*pre->TEMPERATUR_MULT, preset->GRID_SIZE, phi)
{
	//Presets
	preset = pre;
    
    phi.grid->setTransformation(u.getTrans());
	//MeshToVolumeConverter converter;
	//converter.convert("cube.obj", preset->GRID_SIZE);
	//phi.fillLevelSetFromConverter(converter);
	//phi.fillLevelSet(preset->implicitFunction); 
	
	addFuelToLevelSet(phi.grid->xdim()/2, 1.5/phi.grid->dx(), phi.grid->zdim()/2, 0.6/phi.grid->dx()); // if we want to start with a fluid

	//2D grid
	u = MACGrid::createRandom3D(preset->GRID_DIM_X, preset->GRID_DIM_Y,preset->GRID_DIM_Z, preset->GRID_SIZE);
    u_burnt = MACGrid::createRandom3D(preset->GRID_DIM_X, preset->GRID_DIM_Y,preset->GRID_DIM_Z, preset->GRID_SIZE);
	u_fuel = MACGrid::createRandom3D(preset->GRID_DIM_X, preset->GRID_DIM_Y,preset->GRID_DIM_Z, preset->GRID_SIZE);
	enforceBorderCondition();
    
    //Solids
    setSolids();
	//Advect
    
	p = new GridField<double>(phi.grid->xdim(), phi.grid->ydim(), phi.grid->zdim(), FirePresets::GRID_SIZE, new ConstantValueExtrapolation<double>()); //TODO KORREKT EXTRAPOLERING?
	rhs = new GridField<double>(phi.grid->xdim(), phi.grid->ydim(), phi.grid->zdim(), FirePresets::GRID_SIZE, new ConstantValueExtrapolation<double>()); //TODO KORREKT EXTRAPOLERING?
	pVec.reserve( phi.grid->xdim() * phi.grid->ydim() * phi.grid->zdim() );
	rhsVec.reserve( phi.grid->xdim() * phi.grid->ydim() * phi.grid->zdim() );
    
    
	//A = new SparseMatrix<double>(matDim, 7); // Total matrix, antal icke-zeros per rad
	pcgSolver = new PCGSolver<double>();
	resid_out = new double();
	iter_out = 100;
    
	_borderCondition = new BorderCondition();
    
	preset->upwindDisc->setVelocityField(w);
    
	T = new Temperature(phi.grid);
    
	vorticityForces = new GridField<Vector3>(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z, FirePresets::GRID_SIZE, new ConstantValueExtrapolation<Vector3>()); //TODO KORREKT EXTRAPOLERING?
    
	blackBodyRender = BlackBodyRadiation(380, 380, *T->grid);
    
    //Init marker-particles
    for (GridFieldIterator<bool> iter = solids.iterator(); !iter.done(); iter.next()) {
		int i,j,k;
        iter.index(i, j, k);
        double l_x,l_y,l_z;
        solids.indexToLocal(i, j, k, l_x, l_y, l_z);
        double r = 0.2;
        double c_x = 0.5;
        double c_y = 0.25;
        
        if (r*r > (l_x-c_x)*(l_x-c_x)+(l_y-c_y)*(l_y-c_y)+l_z*l_z && iter.value() != SOLID) {
            double x,y,z;
            solids.indexToWorld(i, j, k, x, y, z);
            particles.push_back(Vector3(x,y,z));
        }
    }
    
    ghost.makeRandom();
}

void Fire3D::setSolids(){
    
    for (GridFieldIterator<bool> it = solids.iterator(); !it.done(); it.next()) {
        int i,j,k;
        it.index(i, j, k);
        if ( j == 0 || i == 0 || i == solids.xdim()-1 || k == 0 || k == solids.zdim()-1) {
            solids.setValueAtIndex(true, it.index());
        }else{
            solids.setValueAtIndex(false, it.index());
        }
    }
    
    
    
}

void Fire3D::computeGhostValues(){
    assert(u_fuel == u_burnt);
    for (int field = 0 ; field < 3; field++) {
        GridField<double> *burnt,*fuel;
        VelocityDirection dir;
        if (field == 0) {
            dir = UDIR;
            burnt = u_burnt._u;
            fuel = u_fuel._u;
        }else if (field == 1){
            dir = VDIR;
            burnt = u_burnt._v;
            fuel = u_fuel._v;
        }else { //Field == 2
            burnt = u_burnt._w;
            fuel = u_fuel._w;
            dir = WDIR;
        }
        
        //U,V or W
		
        for (GridFieldIterator<double> it = burnt->iterator(); !it.done(); it.next()) {
            
            int i,j,k;
            it.index(i, j, k);
            double x,y,z; //World coord for face
            burnt->indexToWorld(i, j, k, x, y, z);
            
			//Beräkningar enl. ekv (13) Fedkiw 2002
            CellType cellType = LevelSet::getCellType(phi.grid->valueAtWorld(x, y, z));
            if (cellType == FUEL) {
                
				Vector3 centerVel = u_fuel.velocityAtWorld(Vector3(x,y,z));
				double prod = centerVel.dot(phi.getNormal(x,y,z));
				Vector3 normalProd = prod * phi.getNormal(x,y,z);
                
                double DVn = JumpCondition::DVn(&phi, dir, x, y, z, cellType,BURNT);
                double fuelVal = fuel->valueAtIndex(it.index());
                
				if(dir == UDIR)
					burnt->setValueAtIndex(fuelVal + DVn - normalProd.x,it.index());
				else if (dir == VDIR)
					burnt->setValueAtIndex(fuelVal + DVn - normalProd.y,it.index());
				else //if (dir == WDIR)
                    
					burnt->setValueAtIndex(fuelVal + DVn - normalProd.z,it.index());
				//burnt->setValueAtIndex(fuelVal+DVn,it.index());
                
            }else if (cellType == BURNT){
                
				Vector3 centerVel = u_burnt.velocityAtWorld(Vector3(x,y,z));
				double prod = centerVel.dot(phi.getNormal(x,y,z));
				Vector3 normalProd = prod * phi.getNormal(x,y,z);
                
                double DVn = JumpCondition::DVn(&phi,dir, x, y, z, cellType,FUEL);
                double gasVal = burnt->valueAtIndex(it.index());
                
				if(dir == UDIR)
					fuel->setValueAtIndex(gasVal + DVn - normalProd.x,it.index());
				else if (dir == VDIR)
					fuel->setValueAtIndex(gasVal + DVn - normalProd.y,it.index());
				else //if (dir == WDIR)
					fuel->setValueAtIndex(gasVal + DVn - normalProd.z,it.index());
                //fuel->setValueAtIndex(gasVal+ DVn,it.index());
                
            }
            
        }
    }
    
}

double Fire3D::computeDT(double currentTime){
	double smallStep;
    
	//Bridson s. 35
	double dx = u_fuel.dx(); //TODO KORREKT DX?
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

void Fire3D::advectLevelSet(double duration)
{
	computeW();
	preset->advection->advect(w, phi, duration);
}

void Fire3D::advectTemperature(double dt){
    T->AdvectTemperatureField(dt, u_burnt, phi);
    T->CalculateBuoyancyForceField(phi);
}

double Fire3D::getAlpha(const int i, const int j, const int k, DirectionEnums d){
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

double Fire3D::getDensity(const int i, const int j, const int k, DirectionEnums d){
	// sid. 104 (Bridson)
	double alpha = getAlpha(i,j,k,d);
	CellType temp;
    
	if(d == LEFT)
		temp = phi.getCellType(i-1,j,k);
	if(d == RIGHT)
		temp = phi.getCellType(i+1,j,k);
	if(d == DOWN)
		temp = phi.getCellType(i,j-1,k);
	if(d == UP)
		temp = phi.getCellType(i,j+1,k);
	if(d == BACKWARD)
		temp = phi.getCellType(i,j,k-1);
	if(d == FORWARD)
		temp = phi.getCellType(i,j,k+1);
    
	if(phi.getCellType(i,j,k) == FUEL && temp == FUEL)
		return preset->rhof;
	else if(phi.getCellType(i,j,k) == FUEL && temp == BURNT)
		return alpha * preset->rhof + ( 1 - alpha ) * preset->rhob;
	else if(phi.getCellType(i,j,k) == BURNT && temp == FUEL)
		return alpha * preset->rhob + ( 1 - alpha ) * preset->rhof;
	else if(phi.getCellType(i,j,k) == BURNT && temp == BURNT)
		return preset->rhob;
}

void Fire3D::addFuelToLevelSet(int x0, int y0, int z0, double radius){
	for(int x = 0; x < phi.grid->xdim(); ++x)
	{
		for(int y = 0; y < phi.grid->ydim(); ++y)
		{
			for(int z = 0; z < phi.grid->zdim(); ++z)
			{
				double ndist = radius - sqrt(pow(x0-x, 2.0) + pow(y0-y, 2.0) + pow(z0 - z, 2.0));
				double odist = phi.grid->valueAtIndex(x, y, z);
				double dist;
				if(ndist >= 0.0)
				{
					if(odist < 0.0)
						dist = ndist;
					else//both positive = inside fuel
						dist = min(ndist, odist);
                    
					// force the fueld velocity to point upwards.
					const double amount = 30.0;
					u_fuel.setValueAtFace(0.0, x, y, 0, RIGHT);
					u_fuel.setValueAtFace(0.0, x, y, 0, LEFT);
					u_fuel.setValueAtFace(amount, x, y, 0, UP);
					u_fuel.setValueAtFace(amount, x, y, 0, DOWN);
				}
				else
				{
					if(odist >= 0.0)
						dist = odist;
					else//both negative = outside fuel
						dist = max(ndist, odist);
				}
				phi.grid->setValueAtIndex(dist, x, y, z);
			}
		}
	}
	computeGhostValues();
}

void Fire3D::runSimulation(){
    
	phi.reinitialize();

	addFuelToLevelSet(phi.grid->xdim()/2, 1.5/phi.grid->dx(), phi.grid->zdim()/2, 0.6/phi.grid->dx());

    phi.updateNormals();
    
#if 0
	static int counter = 0;
    if (++counter < 1) {
    	for(int i = -8; i < 8; i++){
            phi.grid->setValueAtIndex(0.3,preset->GRID_DIM_X/2+i,5,0);
            phi.grid->setValueAtIndex(0.3,preset->GRID_DIM_X/2+i,6,0);
            phi.grid->setValueAtIndex(0.3,preset->GRID_DIM_X/2+i,7,0);
            phi.grid->setValueAtIndex(0.3,preset->GRID_DIM_X/2+i,8,0);
        }
    }
#endif
    
    enforceBorderCondition();
    phi.updateNormals();
    
    computeGhostValues();
#pragma omp parallel sections
	{
#pragma omp section
        {
            preset->advectVelocities->advect(u_burnt, preset->dt);
        }
#pragma omp section
        {
            preset->advectVelocities->advect(u_fuel, preset->dt);
        }
	}
    computeGhostValues();
    enforceBorderCondition();
    
    //Vorticity
	Vorticity::addVorticity(u_burnt, *vorticityForces, preset->VORTICITY_EPSILON_BURNT, u_burnt.dx(), phi.grid->xdim(), phi.grid->ydim(), phi.grid->zdim());
    u_burnt.addForceGrid(*vorticityForces, preset->dt); // Add vorticity forces to velocity field
	Vorticity::addVorticity(u_fuel, *vorticityForces,  preset->VORTICITY_EPSILON_FUEL, u_fuel.dx(), phi.grid->xdim(), phi.grid->ydim(), phi.grid->zdim());
    u_fuel.addForceGrid(*vorticityForces, preset->dt); // Add vorticity forces to velocity field
    computeGhostValues();
    
	advectTemperature(preset->dt); 
	
    u_burnt.addForceGrid(*T->beyonce, preset->dt);
    u_fuel.addForceGrid(*T->beyonce, preset->dt);
    

    try{
        projection.project(&u_burnt,&solids, BURNT, preset->dt);
        
        projection.project(&u_fuel,&solids, FUEL, preset->dt);
        
    }
    catch(std::exception &e){
        std::cout << e.what() << std::endl;
    }
    
    computeGhostValues();
    enforceBorderCondition();

    advectLevelSet(preset->dt);

	//not rendered at the moment and should therefore not be calculated
	//smoke.advectDensityField(preset->dt, u_burnt, phi); 
}


void Fire3D::enforceBorderCondition(){
    
    
    for (GridFieldIterator<bool> it = solids.iterator(); !it.done(); it.next()) {
        if (it.value()) {
            int i,j,k;
            it.index(i, j, k);
            u_fuel.setValueAtFace(0, i, j, k, RIGHT);
            u_fuel.setValueAtFace(0, i, j, k, LEFT);
            u_fuel.setValueAtFace(0, i, j, k, UP);
            u_fuel.setValueAtFace(0, i, j, k, DOWN);
            u_fuel.setValueAtFace(0, i, j, k, FORWARD);
            u_fuel.setValueAtFace(0, i, j, k, BACKWARD);
            
            u_burnt.setValueAtFace(0, i, j, k, RIGHT);
            u_burnt.setValueAtFace(0, i, j, k, LEFT);
            u_burnt.setValueAtFace(0, i, j, k, UP);
            u_burnt.setValueAtFace(0, i, j, k, DOWN);
            u_burnt.setValueAtFace(0, i, j, k, FORWARD);
            u_burnt.setValueAtFace(0, i, j, k, BACKWARD);
        }
    }
}

void Fire3D::drawVorticities(){
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

void Fire3D::drawMAC(MACGrid &grid){
    
	glColor3f(1,1,0);
	for (GridMappingIterator iter = grid.iterator(); !iter.done(); iter.next()) {
		int i,j,k;
		iter.index(i, j, k);
		double x,y,z;
		grid.indexToWorld(i, j, k, x, y, z);
        Vector3 vel = grid.velocityAtWorld(Vector3(x,y,z));
        glBegin(GL_LINE_STRIP);
		glVertex3d(x,y,z);
		glVertex3d(x+vel.x, y+vel.y,z+vel.z);
        glEnd();
    }
}

void Fire3D::drawMAC(MACGrid &grid,CellType cellType, double r,double g,double b){
    
	glColor3f(r,g,b);
    
	for (GridMappingIterator iter = grid.iterator(); !iter.done(); iter.next()) {
		int i,j,k;
		iter.index(i, j, k);
        if (phi.getCellType(i, j, k) == cellType  && !solids.valueAtIndex(i, j, k)) {
            double x,y,z;
            grid.indexToWorld(i, j, k, x, y, z);
            Vector3 vel = grid.velocityAtWorld(Vector3(x,y,z))*0.1;
            glBegin(GL_LINE_STRIP);
            glVertex3d(x,y,z);
            glVertex3d(x+vel.x*0.1, y+vel.y*0.1,z+vel.z*0.1);
            glEnd();
        }
	}
}

void Fire3D::drawBoundary(const GridMapping &g) const{
    double x0,y0,z0;
    double x1,y1,z1;
    g.indexToWorld(0, 0, 0, x0, y0, z0);
    g.indexToWorld(g.xdim()-1, g.ydim()-1, g.zdim()-1, x1, y1, z1);
    double alfa = 1.0;
    glPushMatrix();
    glTranslatef(x0, y0, z0);
    glScalef(x1-x0,x1-x0,x1-x0);
    
    

    glPopMatrix();
    
}
template<class T2>
void Fire3D::drawVoxels(const GridField<T2> &g) const{
    double dx = g.dx();
	glColor3f(1,1,0);
	for (GridFieldIterator<T2> iter = g.iterator(); !iter.done(); iter.next()) {
        //if(iter.value()){
        int i,j,k;
        iter.index(i, j, k);
        
        double v = (g.valueAtIndex(i,j,k) - FirePresets::T_AIR)/(FirePresets::T_MAX - FirePresets::T_AIR);
        double alfa = v;
        
        double x,y,z;
        g.indexToWorld(i, j, k, x, y, z);
        glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(dx,dx,dx);
        glBegin(GL_QUADS);
        
        if(alfa < 0.8){
            
            glColor4f(1.0f, 1.0f, 0.0f,(alfa*0.7)*(alfa*0.7));
        }else{
            glColor4f(0.0f, 0.0f, 1.0f,(alfa*0.7)*(alfa*0.7));
        }
        //Front
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(1.0, 0.0, 0.0);
        glVertex3f(1.0, 1.0, 0.0);
        glVertex3f(0.0, 1.0, 0.0);
        
        //Left
        glVertex3f(1.0, 0.0, 0.0);
        glVertex3f(1.0, 0.0, -1.0);
        glVertex3f(1.0, 1.0, -1.0);
        glVertex3f(1.0, 1.0, 0.0);
        
        //Back
        glVertex3f(1.0, 0.0, -1.0);
        glVertex3f(1.0, 1.0, -1.0);
        glVertex3f(0.0, 1.0, -1.0);
        glVertex3f(0.0, 0.0, -1.0);
        
        //Right
        glVertex3f(0.0, 0.0, -1.0);
        glVertex3f(0.0, 1.0, -1.0);
        glVertex3f(0.0, 1.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        
        //Top
        glVertex3f(0.0, 0.0, -0.0);
        glVertex3f(0.0, 0.0, -1.0);
        glVertex3f(1.0, 0.0, -1.0);
        glVertex3f(1.0, 0.0, 0.0);
        
        //Bottom
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(1.0, 0.0, 0.0);
        glVertex3f(1.0, 0.0, -1.0);
        glVertex3f(0.0, 0.0, -1.0);
        
        glEnd();
        glPopMatrix();
        //}
    }
}
void Fire3D::drawGrid(const GridField<bool> &g) const{
    double dx = g.dx();
	glColor4f(1,1,1,0.1);
	for (GridMappingIterator iter = g.iterator(); !iter.done(); iter.next()) {
		int i,j,k;
		iter.index(i, j, k);
		double x,y,z;
		g.indexToWorld(i, j, k, x, y, z);
        
        glBegin(GL_LINE_STRIP);
		glVertex3d(x-dx,y-dx,z-dx);
		glVertex3d(x+dx,y-dx,z-dx);
        glEnd();
        glBegin(GL_LINE_STRIP);
		glVertex3d(x-dx,y-dx,z-dx);
		glVertex3d(x-dx,y+dx,z-dx);
        glEnd();
        glBegin(GL_LINE_STRIP);
		glVertex3d(x-dx,y-dx,z-dx);
		glVertex3d(x-dx,y-dx,z+dx);
        glEnd();
        
        if (i == g.xdim()-1) {
            glBegin(GL_LINE_STRIP);
            glVertex3d(x-dx,y-dx,z-dx);
            glVertex3d(x+dx,y-dx,z-dx);
            glEnd();
        }
        if (j == g.ydim()-1) {
            glBegin(GL_LINE_STRIP);
            glVertex3d(x-dx,y-dx,z-dx);
            glVertex3d(x+dx,y-dx,z-dx);
            glEnd();
        }
        if (k == g.zdim()-1) {
            glBegin(GL_LINE_STRIP);
            glVertex3d(x-dx,y-dx,z-dx);
            glVertex3d(x+dx,y-dx,z-dx);
            glEnd();
        }
        
    }
}

void Fire3D::drawSolid(){
    
	double dx = solids.dx();
	double dy = solids.dy();
	//double dz = celltype.mapping.dx();
	glBegin(GL_QUADS);
	for (GridFieldIterator<bool> iter = solids.iterator(); !iter.done(); iter.next()) {
		double x,y,z;
		int i,j,k;
		iter.index(i, j, k);
        
		solids.indexToWorld(i, j, k, x, y, z);
		int val = iter.value();
        
		if (val){
			glColor3d(1.0, 1.0, 1.0);
			glVertex3d(x-dx*0.5, y-dy*0.5, 0);
			glVertex3d(x+dx*0.5, y-dy*0.5, 0);
			glVertex3d(x+dx*0.5, y+dy*0.5, 0);
			glVertex3d(x-dx*0.5, y+dy*0.5, 0);
		}
	}
    
	glEnd();
}

void Fire3D::drawFaceVelocities(){
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

void Fire3D::drawFaceVelocities(MACGrid &grid) const{
    glColor3f(0,1,0);
	for (GridFieldIterator<double> iter = grid._u->iterator(); !iter.done(); iter.next()) {
		int i,j,k;
		iter.index(i, j, k);
		double x,y,z;
		grid._u->indexToWorld(i, j, k, x, y, z);
		double val = iter.value();
        
		glBegin(GL_LINE_STRIP);
		glVertex3d(x, y, 0);
		glVertex3d(x+val, y, 0);
		glEnd();
	}
    
	glColor3f(1,0,0);
	for (GridFieldIterator<double> iter = grid._v->iterator(); !iter.done(); iter.next()) {
		int i,j,k;
		iter.index(i, j, k);
		double x,y,z;
		grid._v->indexToWorld(i, j, k, x, y, z);
		double val = iter.value();
		glBegin(GL_LINE_STRIP);
		glVertex3d(x, y, 0);
		glVertex3d(x, y+val, 0);
		glEnd();
	}
}

void Fire3D::drawCenterGradients(Discretization *disc)
{
	double max = 0.0;
	for (GridMappingIterator iter = u.iterator(); !iter.done(); iter.next()) {
		int i,j,k;
		iter.index(i, j, k);
		double x,y,z;
		u.indexToWorld(i, j, k, x, y, z);
		GridField<double> *g = phi.grid;
		Vector3 v = Gradient::getGradient(*g, i, j, k, *disc);
        
		if(v.norm() > max)
			max = v.norm();
        
		v *= 0.2;
        
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
        
        glPointSize(0.5f);
        glColor3d(1.0,0.0,1.0);
		glBegin(GL_POINTS);
		glVertex3d(x, y, 0);
		glEnd();
        
        
		glBegin(GL_LINE_STRIP);
		glVertex3d(x, y, 0);
		glVertex3d(x + v.x, y+v.y , 0);
		glEnd();
        
		
		glColor3f(1,0,0);
		glBegin(GL_POINTS);
		glVertex3f(x, y, 0);
		glEnd();
		
        
	}
    
	std::cout << "Max gradient = " << max << std::endl;
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

void Fire3D::drawCenterVelocities(){
    glBegin(GL_LINES);
    for (GridMappingIterator iter = w.iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        w.indexToWorld(i, j, k, x, y, z);
        
        Vector3 v = w.valueAtWorld(x,y,z);//*FirePresets::dx;
        glColor3d(1.0,1.0,0.0);
        glVertex3d(x, y, z);
        glVertex3d(x + v.x, y +v.y , z+v.z);
    }
    glEnd();
}


void Fire3D::drawNormals() const{
    glBegin(GL_LINES);
    for (GridMappingIterator iter = phi.normals->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        phi.normals->indexToWorld(i, j, k, x, y, z);
        
        Vector3 v = phi.normals->valueAtIndex(iter.index());
        glColor3d(1.0,1.0,0.0);
        glVertex3d(x, y, z);
        glVertex3d(x + v.x, y +v.y , z+v.z);
    }
    glEnd();
}

/*void Fire3D::computeW()
 {
 for(GridFieldIterator<Vector3> it = w.iterator(); !it.done(); it.next())
 {
 int i, j, k;
 it.index(i,j,k);
 //Vector3 v = u.velocityAtCenter(i, j, k) + phi.getNormal(i, j, k)*FirePresets::S;
 Vector3 v = phi.getFlameSpeed(i, j, k, &u);
 w.setValueAtIndex(v, it.index());
 }*/



void Fire3D::computeW(){
    for(GridFieldIterator<Vector3> it = w.iterator(); !it.done(); it.next()){
        int i, j, k;
        it.index(i,j,k);
        double x,y,z;
        w.indexToWorld(i, j, k, x, y, z);
        Vector3 v;
        //if (getCellType(i, j, k) == FUEL) {
		//v = u_fuel.velocityAtWorld(Vector3(x,y,z)); Finns en liten risk att detta är korrekta def, men jag tror inte det //Axel
        v = u_fuel.velocityAtWorld(Vector3(x,y,z)) + phi.getNormal(i, j, k)*FirePresets::S;
        /*}else if (getCellType(i, j, k) == BURNT){
         v = u_burnt.velocityAtWorld(Vector3(x,y,z));
         }*/
        
        w.setValueAtIndex(v, it.index());
    }
}

void Fire3D::draw(){

	glLoadIdentity();
	blackBodyRender.draw(*(T->grid), phi, smoke);
    //phi.draw();
    //T->draw();
    /*static double angle = 5*4.2;
    angle += 0.1;
    double dx = solids.dx();
    glTranslatef(dx*0.5, dx*0.5, dx*0.5);
    glRotatef(angle, 1.0, 0.3, 0.0);
    glTranslatef(-dx*0.5, -dx*0.5, -dx*0.5);
    glScalef(0.08, 0.08, 0.08);
    glTranslatef(-8, -8, 0);*/

	//T->drawBuoyancyForce();
	//drawVorticities();
	//drawCenterVelocities();
    //drawMAC(u_burnt, BURNT, 1,0,0);
    //drawMAC(u_fuel, FUEL, 0,1,1);
    //drawGrid(solids);
    //drawBoundary(solids);

    //drawVoxels(*(phi.grid));
    //drawVoxels(*(T->grid));
    //drawMAC(u_fuel, BURNT, 0,1,1);
    //particles.draw();
    //drawMAC(u_burnt, FUEL, 1,0,0);
    //drawMAC(u_burnt, BURNT, 0,1,1);
    
    //drawCenterGradients(FirePresets::centralDisc);
    //drawFaceVelocities(u_burnt);
    
    //drawMAC();
    //drawSolid();
    //drawNormals();
    /*GhostMAC ghost(phi, u);
     Vector3 vector1 = u.velocityAtWorld(Vector3(Input::worldX, Input::worldY, Input::worldZ));
     std::cout << vector1 << std::endl;
     Vector3 vector = ghost.velocityAtWorld(Vector3(Input::worldX, Input::worldY, Input::worldZ), FUEL);
     std::cout << vector << std::endl << std::endl;
     */
	/*
     Vector3 vector = ghost.velocityAtWorld(Vector3(Input::worldX, Input::worldY, Input::worldZ));// phi.getNormal();
     =======
     
     Vector3 vector = phi.normals->valueAtWorld(Input::worldX, Input::worldY, 0.0f);// phi.getNormal();
     >>>>>>> d1c82b63f97c18ca0d04ddc4b576063068254309
     //Vector3 vector =  u.velocityAtWorld(Vector3(Input::worldX,Input::worldY,0.0f));
     
     
     glEnable(GL_POINT_SMOOTH);
     glPointSize(3.0f);
     glColor3d(0.6, 0.6, 1.0);
     glBegin(GL_LINE_STRIP);
     glVertex3d(Input::worldX,Input::worldY,0.0f);
     glEnd();
     
     glBegin(GL_LINES);
     glVertex3d(Input::worldX,Input::worldY,0.0f);
     glVertex3d(Input::worldX+vector.x,Input::worldY+vector.y,0.0f);
     glEnd();*/
    
}

Fire3D::~Fire3D(){
    
    delete preset;
	delete A;
	delete rhs;
	delete p;
	delete _borderCondition;
}
