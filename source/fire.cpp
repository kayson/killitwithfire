
#include "fire.h"
#include "advect/Advect.h"

#include "GridField.hpp"

Fire::Fire(FirePresets *pre):phi(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z,preset->GRID_SIZE),celltype(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z),u(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z, preset->GRID_SIZE)
{
    //Presets
    preset = pre;

    
    //u.fillVelocity(Vector3(0.1,0.0,0));
	phi.fillLevelSet(preset->implicitFunction);


    preset->advect->setDiscretization(preset->upwindDiscretization, preset->centralDiscretization);
	const int matDim = phi.phi->xdim()*phi.phi->ydim()*phi.phi->zdim()*phi.phi->xdim()*phi.phi->ydim()*phi.phi->zdim();
	//A = new SparseMatrix<double>(matDim, 7); // Total matrix, antal icke-zeros per rad
	_borderCondition = new BorderCondition();

	preset->upwindDiscretization->setMACGrid(&u);
	preset->centralDiscretization->setMACGrid(&u);


	preset->advect->setDiscretization(preset->upwindDiscretization, preset->centralDiscretization);


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
	preset->advect->advect(u, phi.phi,phi.temp, duration);
}

void Fire::project(double dt)
{
	double scale = 1 / preset->dx;

	// b (rhs)
	// sid. 45, Figure 4.2 (Bridson)
	for(int i = 0; i < phi.phi->xdim(); i ++)
		for(int j = 0; j < phi.phi->ydim(); j ++)
			for(int k = 0; k < phi.phi->zdim(); k ++)
			{
				if(getCellType(i,j,k) == BLUECORE || getCellType(i,j,k) == IGNITED)
					rhs->setValueAtIndex(
					(u.valueAtFace(i,j,k,RIGHT) - u.valueAtFace(i,j,k,LEFT) +
					u.valueAtFace(i,j,k,UP) - u.valueAtFace(i,j,k,DOWN) +
					u.valueAtFace(i,j,k,FORWARD) - u.valueAtFace(i,j,k,BACKWARD))
					,i,j,k);
			}


	// A
	
	for(int i = 0; i<u.xdim(); i++){
		for(int j = 0; i<u.ydim(); j++){
			for(int k = 0; k<u.zdim(); k++){
				// Korrekt skalningsfaktor
				if(getCellType(i,j,k) == BLUECORE) // om bränsle
					scale = dt/(preset->dx*preset->dx*preset->rhof);
				else if(getCellType(i,j,k) == IGNITED) // om gas
					scale = dt/(preset->dx*preset->dx*preset->rhoh);

				/*if(!_borderCondition->checkBorder(*phi.phi,i,j,k))
					_borderCondition->enforceBorderCondition(u, *phi.phi,i,j,k);*/
				if(getCellType(i,j,k) == BLUECORE && getCellType(i+1,j,k) == BLUECORE){
					//A->set_element(u.getCenterField()->
				}
				
			}
		}
	}

	// räkna fram nya p mha. A och b

	// räkna fram u^(n+1) med nya p, 
	// sid. 41, Figure 4.1 (Bridson)
	for(int i = 0; i < phi.phi->xdim(); i ++)
		for(int j = 0; j < phi.phi->ydim(); j ++)
			for(int k = 0; k < phi.phi->zdim(); k ++)
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
			}

}

double Fire::getAlpha(const int i, const int j, const int k, DirectionEnums d)
{
	// sid. 104 (Bridson)
	double temp = 0;

	if(d == RIGHT)
		temp = phi.phi->valueAtIndex(i+1,j,k);
	if(d == LEFT)
		temp = phi.phi->valueAtIndex(i-1,j,k);
	if(d == UP)
		temp = phi.phi->valueAtIndex(i,j+1,k);
	if(d == DOWN)
		temp = phi.phi->valueAtIndex(i,j-1,k);
	if(d == FORWARD)
		temp = phi.phi->valueAtIndex(i,j,k+1);
	if(d == BACKWARD)
		temp = phi.phi->valueAtIndex(i,j,k-1);

	if(phi.phi->valueAtIndex(i,j,k) <= 0 && temp <= 0)
		return 1;
	else if(phi.phi->valueAtIndex(i,j,k) <= 0 && temp > 0)
		return (phi.phi->valueAtIndex(i,j,k) / 
			(phi.phi->valueAtIndex(i,j,k) - temp));
	else if(phi.phi->valueAtIndex(i,j,k) > 0 && temp <= 0)
		return 1 - (phi.phi->valueAtIndex(i,j,k) / 
			(phi.phi->valueAtIndex(i,j,k) - temp));
	else if(phi.phi->valueAtIndex(i,j,k) > 0 && temp > 0)
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
        
        if(celltype(i,j,k)){
            
        }   
        
		celltype.setValueAtIndex(getCellType(i,j,k), i, j, k);

	}
}	

CellType Fire::getCellType(const int i, const int j, const int k)
{
	if(false) //Check if is solid
		return SOLID;
	else if(phi.phi->valueAtIndex(i,j,k) <= 0.0)
		return BLUECORE;
	else 
		return IGNITED;
}

int count = 0;
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
		advectLevelSet(preset->dt);

		currentTime += dt;
    }

    u.advect(preset->dt);
    u.addForce(Vector3(0.0, -0.5, 0.0), preset->dt);

	//Beräkna om vad för typ voxlarna är
	computeCellTypes(); 
    
    //Externa krafter  
		//preset->externalForce->addForce(grid);
    
    //Project
    
  	
	//Fixa signed distance field
	/*if(count % 50 == 0)*/
	phi.reinitialize();
	count++;
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
	for(int i = 0; i < phi.phi.xdim(); i += 5)
>>>>>>> Tagit bort vectorgrid
	{
		for(int j = 0; j < phi.phi->ydim(); j += 5)
		{
			for(int k = 0; k < phi.phi->zdim(); k += 5)
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
	drawCenterVelocities();
}

Fire::~Fire(){
    delete preset;
	//delete A;
	delete _borderCondition;
}