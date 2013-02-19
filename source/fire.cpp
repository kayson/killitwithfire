
#include "fire.h"
#include "advect/Advect.h"

#include "GridField.hpp"

#ifdef __APPLE__
#include "glfw.h"
#include "pcg_solver.h"
#include <Accelerate/Accelerate.h>
#elif defined _WIN32 || defined _WIN64
#include <GL/glfw.h>
#include "Pressure/pcgsolver/pcg_solver.h"
#endif

Fire::Fire(FirePresets *pre):phi(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z,preset->GRID_SIZE),celltype(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z),u(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z, preset->GRID_SIZE)
{
    //Presets
    preset = pre;

	phi.fillLevelSet(preset->implicitFunction);


	p = new GridField<double>(phi.grid->xdim(), phi.grid->ydim(), phi.grid->zdim());
	rhs = new GridField<double>(phi.grid->xdim(), phi.grid->ydim(), phi.grid->zdim());
	pVec.reserve( phi.grid->xdim() * phi.grid->ydim() * phi.grid->zdim() );
	rhsVec.reserve( phi.grid->xdim() * phi.grid->ydim() * phi.grid->zdim() );

	const int matDim = phi.grid->xdim()*phi.grid->ydim()*phi.grid->zdim()*phi.grid->xdim()*phi.grid->ydim()*phi.grid->zdim();

	A = new SparseMatrix<double>(matDim, 7); // Total matrix, antal icke-zeros per rad

	resid_out = new double();
	iter_out = 10;

	_borderCondition = new BorderCondition();

	preset->centralDisc->setMACGrid(&u);
	preset->upwindDisc->setMACGrid(&u);

}

double Fire::computeDT(double currentTime){
	double smallStep;

	//Bridson s. 35
	double dx = preset->dx;
	double alpha = preset->CFL_NUMBER;
	double c = 1;// u.getMax();
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
	preset->advection->advect(u, &phi.grid, &phi.gridCopy, duration);
}

void Fire::project2D(double dt){
    double x0,x1,y,z;
    u._center->mapping.indexToWorld(0, 0, 0, x0, y, z);
    u._center->mapping.indexToWorld(0, 0, 0, x1, y, z);

    double dx = x1-x0;
    double scale = 1.0/dx;

	// b (rhs)
	// sid. 45, Figure 4.2 (Bridson)
    std::vector<double> rhs(u._center->mapping.size());
    double mean = 0;
    for (GridFieldIterator<double> iter = u._center->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        if (k == 0) {
            rhs[iter.index()] = (u.valueAtFace(i,j,k,RIGHT) - u.valueAtFace(i,j,k,LEFT)) + (u.valueAtFace(i,j,k,UP) - u.valueAtFace(i,j,k,DOWN));
            mean += rhs[iter.index()];
        }
    }
    mean *= 1.0/((double)rhs.size());
    for (int i = 0; i < rhs.size(); i++) {
        rhs[i] += mean;
    }
    
    
    scale = (1.0 * dx*dx)/dt;

	// A
	unsigned int row = 0;
    for (GridFieldIterator<double> iter = u._center->iterator(); !iter.done(); iter.next()) {
        
        int i,j,k;

        iter.index(i, j, k);
        if (k == 0) {
            double mult = 0;
            if (u._center->mapping.indexAt(i-1, j, k) >= 0) {
                A->set_element(u._center->mapping.indexAt(i-1, j, k), j, scale);
                mult++;
            }
            
            if (u._center->mapping.indexAt(i+1, j, k) < u._center->mapping.size()) {
                A->set_element(u._center->mapping.indexAt(i+1, j, k), j, scale);
                mult++;
            }
            
            if (u._center->mapping.indexAt(i, j+1, k) < u._center->mapping.size()) {
                A->set_element(u._center->mapping.indexAt(i, j+1, k), j, scale);
                mult++;
            }
            
            if (u._center->mapping.indexAt(i, j-1, k) >= 0) {
                A->set_element(u._center->mapping.indexAt(i, j-1, k), j, scale);
                mult++;
            }
            
            A->set_element(u._center->mapping.indexAt(i, j, k), j, mult*scale);


            row++;
        }
	}
    
	// räkna fram nya p mha. A och b
    /*PCGSolver<double> pcgSolver;
	if(!pcgSolver.solve(*A, rhsVec, pVec, *resid_out, iter_out)){
        
    }
    */
    
    //scale = dt/(1.0*dx);

    for (GridFieldIterator<double> iter = u._center->iterator(); !iter.done(); iter.next()) {
        
        int i,j,k;
        iter.index(i, j, k);
        u.setValueAtFace(u.valueAtFace(i, j, k, UP) + pVec[iter.value()], i, j, k, UP);
        u.setValueAtFace(u.valueAtFace(i, j, k, DOWN) + pVec[iter.value()], i, j, k, DOWN);
        u.setValueAtFace(u.valueAtFace(i, j, k, LEFT)  + pVec[iter.value()], i, j, k, LEFT);
        u.setValueAtFace(u.valueAtFace(i, j, k, RIGHT)+ pVec[iter.value()] , i, j, k, RIGHT);

	}

    
}

void Fire::project(double dt)
{
	double scale = 1.0 / preset->dx;

	// b (rhs)
	// sid. 45, Figure 4.2 (Bridson)
	for(int i = 0; i < phi.grid->xdim(); i ++){
		for(int j = 0; j < phi.grid->ydim(); j ++){
			for(int k = 0; k < phi.grid->zdim(); k ++){
				if(getCellType(i,j,k) == BLUECORE || getCellType(i,j,k) == IGNITED)
					rhs->setValueAtIndex(
					(u.valueAtFace(i,j,k,RIGHT) - u.valueAtFace(i,j,k,LEFT) +
					u.valueAtFace(i,j,k,UP) - u.valueAtFace(i,j,k,DOWN) +
					u.valueAtFace(i,j,k,FORWARD) - u.valueAtFace(i,j,k,BACKWARD))
					,i,j,k);
			}
		}
	}

	// Spåna om rhs och b till std::vector<double>
	for(int i = 0; i < phi.grid->xdim(); i ++){
		for(int j = 0; j < phi.grid->ydim(); j ++){
			for(int k = 0; k < phi.grid->zdim(); k ++){
				int index = u.getCenterField()->mapping.indexAt(i,j,k);
				rhsVec[index] = u.getCenterField()->operator()(i,j,k);
			}
		}
	}

	// A
	unsigned int diagRow = 0;
	for(int i = 0; i<u._center->xdim(); i++){
		//std::cout << "Calc Ai\n";
		for(int j = 0; j<u._center->ydim(); j++){
			for(int k = 0; k<u._center->zdim(); k++){

				double x,y,z;
				u._center->mapping.indexToWorld(i,j,k,x,y,z);

				// Korrekt skalningsfaktor
				if( phi.grid->valueAtWorld(x,y,z) < 0 )//if(getCellType(i,j,k) == BLUECORE) // om bränsle
					scale = dt/(preset->dx*preset->dx*preset->rhof);
				else if( phi.grid->valueAtWorld(x,y,z) >= 0 )//else if(getCellType(i,j,k) == IGNITED) // om gas
					scale = dt/(preset->dx*preset->dx*preset->rhoh);
				if(i>0)
					A->set_element(diagRow, u._center->mapping.indexAt(i-1,j,k), u.valueAtFace(i,j,k,LEFT) + scale);
				if(i<u._center->xdim()-1)
					A->set_element(diagRow, u._center->mapping.indexAt(i+1,j,k), u.valueAtFace(i,j,k,RIGHT) + scale);
				if(j<u._center->ydim()-1)
					A->set_element(diagRow, u._center->mapping.indexAt(i,j+1,j), u.valueAtFace(i,j,k,UP) + scale);
				if(j>0)
					A->set_element(diagRow, u._center->mapping.indexAt(i,j-1,k), u.valueAtFace(i,j,k,DOWN) + scale);
				if(k>0)
					A->set_element(diagRow, u._center->mapping.indexAt(i,j,k-1), u.valueAtFace(i,j,k,BACKWARD) + scale);
				if(k<u._center->zdim()-1)
					A->set_element(diagRow, u._center->mapping.indexAt(i,j,k+1), u.valueAtFace(i,j,k,FORWARD) + scale);
				
				A->set_element(diagRow, u._center->mapping.indexAt(i,j,k) , u.valueAtFace(i,j,k,CENTER) + scale);

				++diagRow;
			}
		}
	}

	// räkna fram nya p mha. A och b
    /*PCGSolver<double> pcgSolver;
	if(!pcgSolver.solve(*A, rhsVec, pVec, *resid_out, iter_out)){
        
    }*/

	// räkna fram u^(n+1) med nya p, 
	// sid. 41, Figure 4.1 (Bridson)
    /*for (GridFieldIterator<double> iter = u._center->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double p = pVec[iter.index()];
        u.setValueAtFace(u.valueAtFace(i,j,k,LEFT) - (dt / (getDensity(i,j,k,LEFT) * preset->dx)) * p,i,j,k,LEFT);
        u.setValueAtFace(u.valueAtFace(i,j,k,RIGHT) + (dt / (getDensity(i,j,k,RIGHT) * preset->dx)) * p,i,j,k,RIGHT);
        u.setValueAtFace(u.valueAtFace(i,j,k,DOWN) - (dt / (getDensity(i,j,k,DOWN) * preset->dx)) * p,i,j,k,DOWN);
        u.setValueAtFace(u.valueAtFace(i,j,k,UP) + (dt / (getDensity(i,j,k,UP) * preset->dx)) * p,i,j,k,UP);
        u.setValueAtFace(u.valueAtFace(i,j,k,BACKWARD) - (dt / (getDensity(i,j,k,BACKWARD) * preset->dx)) * p,i,j,k,BACKWARD);
        u.setValueAtFace(u.valueAtFace(i,j,k,FORWARD) + (dt / (getDensity(i,j,k,FORWARD) * preset->dx)) * p,i,j,k,FORWARD);
    }*/
    
    
	/*for(int i = 0; i < phi.grid->xdim(); i ++)
		for(int j = 0; j < phi.grid->ydim(); j ++)
			for(int k = 0; k < phi.grid->zdim(); k ++)
			{
				if(getCellType(i,j,k) == BLUECORE || getCellType(i,j,k) == IGNITED)
				{
					u.setValueAtFace(u.valueAtFace(i,j,k,LEFT) - (dt / (getDensity(i,j,k,LEFT) * preset->dx)) * p->valueAtIndex(i,j,k),i,j,k,LEFT);
					u.setValueAtFace(u.valueAtFace(i,j,k,RIGHT) + (dt / (getDensity(i,j,k,RIGHT) * preset->dx)) * p->valueAtIndex(i,j,k),i,j,k,RIGHT);
					u.setValueAtFace(u.valueAtFace(i,j,k,DOWN) - (dt / (getDensity(i,j,k,DOWN) * preset->dx)) * p->valueAtIndex(i,j,k),i,j,k,DOWN);
					u.setValueAtFace(u.valueAtFace(i,j,k,UP) + (dt / (getDensity(i,j,k,UP) * preset->dx)) * p->valueAtIndex(i,j,k),i,j,k,UP);
					u.setValueAtFace(u.valueAtFace(i,j,k,BACKWARD) - (dt / (getDensity(i,j,k,BACKWARD) * preset->dx)) * p->valueAtIndex(i,j,k),i,j,k,BACKWARD);
					u.setValueAtFace(u.valueAtFace(i,j,k,FORWARD) + (dt / (getDensity(i,j,k,FORWARD) * preset->dx)) * p->valueAtIndex(i,j,k),i,j,k,FORWARD);

				}
				else if(getCellType(i,j,k) == SOLID)
				{

				}
			}*/
     
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

CellType Fire::getCellType(const int i, const int j, const int k)
{
	if(false) //Check if is solid
		return SOLID;
	else if(phi.grid->valueAtIndex(i,j,k) > 0.0)
		return BLUECORE;
	else 
		return IGNITED;
}

void Fire::runSimulation(){
    
    //Beräkna tidssteget
	for(double currentTime = 0; currentTime < preset->dt;)
	{
		double dt = computeDT(currentTime);

		currentTime += dt;
	}	


	 //Advektera levelset
    for(double currentTime = 0; currentTime < preset->dt;)
	{
		double dt = computeDT(currentTime);

		//Advektera hastighestsfältet
		//advectLevelSet(preset->dt);

		currentTime += dt;
    }


    advectLevelSet(preset->dt);
    u.advect(preset->dt);
    Vector3 force = Vector3(0.0, -0.1, 0.0);
    u.addForce(force, preset->dt);

	//Beräkna om vad för typ voxlarna är
	computeCellTypes(); 
    
    //Externa krafter  
		//preset->externalForce->addForce(grid);
    
    //Project

	//project2D(preset->dt);

  	
	//Fixa signed distance field
	//phi.reinitialize();
}

void Fire::drawCenterVelocities()
{
    
    for (GridFieldIterator<double> iter = u._center->iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        u._center->mapping.indexToWorld(i, j, k, x, y, z);
        
        Vector3 v = u.velocityAtWorld(Vector3(x,y,z));//*FirePresets::dx;
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
                    
        glColor3f(0,1,0);
        glBegin(GL_LINES);
        glVertex3f(x, y, 0);
        glVertex3f(x + v.x, y + v.y, 0);
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

void Fire::draw()
{
	phi.draw();
    //u.draw();
	//drawCenterVelocities();
}

Fire::~Fire(){
    delete preset;
	//delete pcgSolver;
	delete A;
	delete rhs;
	delete p;
	delete _borderCondition;
}