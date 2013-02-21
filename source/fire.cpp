
#include <iomanip>
#include "fire.h"

#include "advect/AdvectLevelSet.h"
#include "GridField.hpp"

#ifdef __APPLE__
#include "glfw.h"
#include "pcg_solver.h"
#include <Accelerate/Accelerate.h>
#include "transform.hpp"
#elif defined _WIN32 || defined _WIN64
#include <GL/glfw.h>

#endif

Fire::Fire(FirePresets *pre):phi(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z,preset->GRID_SIZE), w(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z,preset->GRID_SIZE),celltype(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z),u(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_DIM_Z, preset->GRID_SIZE)
{
    //Presets
    preset = pre;

	phi.fillLevelSet(preset->implicitFunction);
    //2D grid
    u = MACGrid::createRandom2D(preset->GRID_DIM_X, preset->GRID_DIM_Y, preset->GRID_SIZE);

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
	computeW();
	preset->advection->advect(u, &phi.grid, &phi.gridCopy, duration);
}

void Fire::project2D(double dt){
    double rho = 0.5;
    double dx = u.dx();
    double residual = -1;
    int iterations = -1;
    int cells = u.size();
	// b (rhs)
	// sid. 45, Figure 4.2 (Bridson)
    static std::vector< double> b(cells);
    static std::vector< double > p(cells);
    static SparseMatrix<double> A(cells,4);
    
    //Fill b
    double mean = 0;
    for (GridMappingIterator iter = u.iterator(); !iter.done();iter.next()) {
        double scale = 1.0/dx;
        int i,j,k;
        double x,y,z;
        iter.index(i, j, k);
        if (k == 0) {

            u.indexToWorld(i, j, k, x, y, z);
            
            double left = u.valueAtFace(i,j,k, LEFT);
            double right = u.valueAtFace(i,j , k, RIGHT);
            double down = u.valueAtFace(i,j , k, DOWN);
            double up = u.valueAtFace(i,j , k, UP);
            
            if (celltype.valueAtWorld(x, y, z) == BLUECORE) {
                b[iter.index()] = -scale*(right-left+up-down);
                mean += b[iter.index()];
            }else{
                b[iter.index()] = 0;
            }
        }

        /*
        scale = 1 / dx;
        loop over i,j,k where cell(i,j,k)==FLUID:
        rhs(i,j,k) = -scale * (u(i+1,j,k)-u(i,j,k) +v(i,j+1,k)-v(i,j,k)
                               +w(i,j,k+1)-w(i,j,k));
         */
        
    }
    mean /= (double)cells;
    for (GridMappingIterator iter = u.iterator(); !iter.done();iter.next()) {
        int i,j,k;
        double x,y,z;
        iter.index(i, j, k);
        if (k == 0) {
            u.indexToWorld(i, j, k, x, y, z);
            if (celltype.valueAtWorld(x, y, z) == BLUECORE) {
                b[iter.index()] -= mean;
            }else{
            }
        }
        
        /*
         scale = 1 / dx;
         loop over i,j,k where cell(i,j,k)==FLUID:
         rhs(i,j,k) = -scale * (u(i+1,j,k)-u(i,j,k) +v(i,j+1,k)-v(i,j,k)
         +w(i,j,k+1)-w(i,j,k));
         */
        
    }
    
    //Ax=b
    /*std::cout << std::endl;
    for (int i = 0 ; i < cells; i++) {
        std::cout << "|" << b[i] << "|" << std::endl;
    }*/
    
    
    int row = 0;
    for (GridMappingIterator iter = u.iterator(); !iter.done();iter.next()) {
        double scale = dt/(rho*dx*dx);

        int i,j,k;
        //double x,y,z;
        iter.index(i, j, k);
        if (k == 0) {
            //u.indexToWorld(i, j, k, x, y, z);
            
            if (celltype.valueAtIndex(i, j, k) == BLUECORE){
                
                if (celltype.indexAt(i+1, j, k) < cells &&celltype.valueAtIndex(i+1, j, k) == BLUECORE) {
                    A.add_to_element(celltype.indexAt(i+1, j, k), row, -scale);
                    A.add_to_element(row, row, scale);
                }else if (celltype.indexAt(i+1, j, k) < cells &&celltype.valueAtIndex(i+1, j, k) == AIR){
                    A.add_to_element(row, row, scale);
                }
                
                if (celltype.indexAt(i-1, j, k) >= 0 &&  celltype.valueAtIndex(i-1, j, k) == BLUECORE){
                    A.add_to_element(celltype.indexAt(i-1, j, k), row, -scale);
                    A.add_to_element(row, row, scale);
                }else if (celltype.indexAt(i-1, j, k) >= 0 &&  celltype.valueAtIndex(i-1, j, k) == AIR){
                    A.add_to_element(row, row, scale);
                }
                
                if (celltype.indexAt(i, j+1, k) < cells && celltype.valueAtIndex(i, j+1, k) == BLUECORE){
                    A.add_to_element(celltype.indexAt(i, j+1, k), row, -scale);
                    A.add_to_element(row, row, scale);
                }else if (celltype.indexAt(i, j+1, k) < cells && celltype.valueAtIndex(i, j+1, k) == AIR){
                    A.add_to_element(row, row, scale);
                }
                
                if (celltype.indexAt(i, j-1, k) >= 0 && celltype.valueAtIndex(i, j-1, k) == BLUECORE){
                    A.add_to_element(celltype.indexAt(i, j-1, k), row, -scale);
                    A.add_to_element(row, row, scale);
                }else if (celltype.indexAt(i, j-1, k) >= 0 && celltype.valueAtIndex(i, j-1, k) == AIR){
                    A.add_to_element(row, row, scale);
                }

            }else{
                A.add_to_element(row, row, 0);
            }
            
            row++;

        }

        
        /*
         scale = dt / (rho*dx*dx); loop over i,j,k:
         if cell(i,j,k)==FLUID and cell(i+1,j,k)==FLUID: Adiag(i,j,k) += scale;
         Adiag(i+1,j,k) += scale;
         Aplusi(i,j,k) = -scale;
         else if cell(i,j,k)==FLUID and cell(i+1,j,k)==EMPTY: Adiag(i,j,k) += scale;
         if cell(i,j,k)==FLUID and cell(i,j+1,k)==FLUID: Adiag(i,j,k) += scale;
         Adiag(i,j+1,k) += scale;
         Aplusj(i,j,k) = -scale;
         else if cell(i,j,k)==FLUID and cell(i,j+1,k)==EMPTY: Adiag(i,j,k) += scale;
         if cell(i,j,k)==FLUID and cell(i,j,k+1)==FLUID: Adiag(i,j,k) += scale;
         Adiag(i,j,k+1) += scale;
         Aplusk(i,j,k) = -scale;
         else if cell(i,j,k)==FLUID and cell(i,j,k+1)==EMPTY: Adiag(i,j,k) += scale;
         */
        
    }
    
    
    //Ax=b
    /*std::cout << std::endl;
    for (int i = 0 ; i < cells; i++) {
        std::cout << "|";
        for (int j = 0 ; j < cells; j++) {
            std::cout << std::setw(3) << A(i,j) << " ";
        }
        std::cout << "| |x" << i << "| = |" << std::setw(1) << b[i] << "|" << std::endl;
    }*/
    
	// räkna fram nya p mha. A och b
    PCGSolver<double> pcgSolver;
	if(!pcgSolver.solve(A, b, p, residual, iterations)){
        
    }
    
    
    for (GridMappingIterator iter = u.iterator(); !iter.done();iter.next()) {
        
        double scale = dt / (rho*dx);
        int i,j,k;
        double x,y,z;
        iter.index(i, j, k);
        if (k == 0) {

            u.indexToWorld(i, j, k, x, y, z);
            if (celltype.valueAtWorld(x, y, z) == BLUECORE) {
                //std::cout << p[iter.index()] << std::endl;
                u.addValueAtFace(-scale*p[iter.index()], i, j, k, LEFT);
                u.addValueAtFace(scale*p[iter.index()], i, j, k, RIGHT);
                u.addValueAtFace(scale*p[iter.index()], i, j, k, UP);
                u.addValueAtFace(-scale*p[iter.index()], i, j, k, DOWN);
            }
        }

        
    
    }
    
    /*std::cout << std::endl;
    for (int i = 0 ; i < cells; i++) {
        std::cout << "|x" << i << "| = |" << std::setw(1) << b[i] << "|" << std::endl;
    }*/
    /*
    loop over i,j,k where cell(i,j,k)==FLUID:
    u(i,j,k) -= scale * p(i,j,k); 
     u(i+1,j,k) += scale * p(i,j,k);
     v(i,j,k) -= scale * p(i,j,k);
     v(i,j+1,k) += scale * p(i,j,k);
     w(i,j,k) -= scale * p(i,j,k); 
     w(i,j,k+1) += scale * p(i,j,k);*/

    
}
/*
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
				int index = u.indexAt(i,j,k);
				rhsVec[index] = u(i,j,k);
			}
		}
	}

	// A
	unsigned int diagRow = 0;
	for(int i = 0; i<u.xdim(); i++){
		//std::cout << "Calc Ai\n";
		for(int j = 0; j<u.ydim(); j++){
			for(int k = 0; k<u..zdim(); k++){

				double x,y,z;
				u.indexToWorld(i,j,k,x,y,z);

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
	}*/

	// räkna fram nya p mha. A och b
    /*PCGSolver<double> pcgSolver;*/
	/*if(!pcgSolver->solve(*A, rhsVec, pVec, *resid_out, iter_out)){
        std::cout << "TRYCK OK!!\n";
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
     
//}

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

	//Beräkna om vad för typ voxlarna är
	computeCellTypes(); 

    u.advect(preset->dt);
    Vector3 force = Vector3(0.0, -0.1, 0.0);
    u.addForce(force, preset->dt);

	advectLevelSet(preset->dt);

    //Externa krafter  
		//preset->externalForce->addForce(grid);
    
    //Project
	//project(preset->dt);
	//project2D(preset->dt);

  	
	//Fixa signed distance field
	//phi.reinitialize();
}

void Fire::drawMAC(){
    
    glColor3f(1,1,0);
    glBegin(GL_POINTS);
    for (GridMappingIterator iter = u.iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        u.indexToWorld(i, j, k, x, y, z);

        glVertex3f(x, y, 0);
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
                glColor3f(0.1, 0.1, 0.1);
                glVertex3f(x-dx*0.5, y-dy*0.5, 0);
                glVertex3f(x+dx*0.5, y-dy*0.5, 0);
                glVertex3f(x+dx*0.5, y+dy*0.5, 0);
                glVertex3f(x-dx*0.5, y+dy*0.5, 0);
            }else if (val == BLUECORE){
                glColor3f(0, 0, 1);
                glVertex3f(x-dx*0.5, y-dy*0.5, 0);
                glVertex3f(x+dx*0.5, y-dy*0.5, 0);
                glVertex3f(x+dx*0.5, y+dy*0.5, 0);
                glVertex3f(x-dx*0.5, y+dy*0.5, 0);
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
        glVertex3f(x, y, 0);
        glVertex3f(x+val, y, 0);
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
        glVertex3f(x, y, 0);
        glVertex3f(x, y+val, 0);
        glEnd();
    }
    
}

void Fire::drawCenterVelocities()
{

    for (GridMappingIterator iter = u.iterator(); !iter.done(); iter.next()) {
        int i,j,k;
        iter.index(i, j, k);
        double x,y,z;
        u.indexToWorld(i, j, k, x, y, z);
        
        

        
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
                    
        glColor3f(1,1,0);
        glBegin(GL_LINE_STRIP);
        glVertex3f(x, y, 0);
        glVertex3f(x + v.x, y+v.y , 0);
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
    //u.draw();
	drawCenterVelocities();
    drawFaceVelocities();
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