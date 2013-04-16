/*************************************************************************************************
*
* Modeling and animation (TNM079) 2007
* Code base for lab assignments. Copyright:
*   Gunnar Johansson (gunnar.johansson@itn.liu.se)
*   Ken Museth (ken.museth@itn.liu.se)
*   Michael Bang Nielsen (bang@daimi.au.dk)
*   Ola Nilsson (ola.nilsson@itn.liu.se)
*   Andreas Sˆderstrˆm (andreas.soderstrom@itn.liu.se)
*
*************************************************************************************************/
#include "LevelSetOperator.h"


/*! Computes the squares of the partial derivatives in x, y, z using the
 * Godunov method
 *
 * \f[
 * \left( \dfrac{\partial\phi}{\partial x} \right)^2 \approx \begin{cases}
 * \text{max}\left[\text{max}(\phi_x^-,0)^2,       \text{min}(\phi_x^+,0)^2\right] & F > 0 \\
 * \text{max}\left[\text{min}(\phi_x^-,0)^2,       \text{max}(\phi_x^+,0)^2\right] & F < 0 \\
 * \end{cases}
 * \f]
 *
 * \param[in] i grid x coordinate
 * \param[in] j grid y coordinate
 * \param[in] k grid z coordinate
 * \param[in] a speed function
 * \param[out] ddx2 (dphi/dx)^2
 * \param[out] ddy2 (dphi/dy)^2
 * \param[out] ddz2 (dphi/dy)^2
 */
void LevelSetOperator::Godunov(unsigned int i, unsigned int j, unsigned int k, float a,
                               float & ddx2, float & ddy2, float & ddz2)
{
  float ddxm = mLS->DiffXm(i,j,k);
  float ddxp = mLS->DiffXp(i,j,k);
  float ddym = mLS->DiffYm(i,j,k);
  float ddyp = mLS->DiffYp(i,j,k);
  float ddzm = mLS->DiffZm(i,j,k);
  float ddzp = mLS->DiffZp(i,j,k);

  if (a > 0) {
    ddx2 = std::max( std::pow(std::max(ddxm,0.0f),2), std::pow(std::min(ddxp,0.0f),2) );
    ddy2 = std::max( std::pow(std::max(ddym,0.0f),2), std::pow(std::min(ddyp,0.0f),2) );
    ddz2 = std::max( std::pow(std::max(ddzm,0.0f),2), std::pow(std::min(ddzp,0.0f),2) );
  }
  else {
    ddx2 = std::max( std::pow(std::min(ddxm,0.0f),2), std::pow(std::max(ddxp,0.0f),2) );
    ddy2 = std::max( std::pow(std::min(ddym,0.0f),2), std::pow(std::max(ddyp,0.0f),2) );
    ddz2 = std::max( std::pow(std::min(ddzm,0.0f),2), std::pow(std::max(ddzp,0.0f),2) );
  }
}


void LevelSetOperator::IntegrateEuler(float dt)
{
  // Create grid used to store next time step
  LevelSetGrid grid = GetGrid();



  // Iterate over grid and compute the grid values for the next timestep
  LevelSetGrid::Iterator iter = GetGrid().BeginNarrowBand();
  LevelSetGrid::Iterator iend = GetGrid().EndNarrowBand();
  while (iter != iend) {
    unsigned int i = iter.GetI();
    unsigned int j = iter.GetJ();
    unsigned int k = iter.GetK();

    // Compute rate of change
    float ddt = Evaluate(i,j,k);

    // Compute the next time step and store it in the grid
    grid.SetValue(i,j,k, GetGrid().GetValue(i,j,k) + ddt*dt);

    iter++;
  }


  // Update the grid with the next time step
  GetGrid() = grid;
}


void LevelSetOperator::IntegrateRungeKutta(float dt)
{
	/*
		TVD RK
	*/
	/*
	//Spara värdet för n samt beräkna n+1
  LevelSetGrid grid = GetGrid();
  LevelSetGrid gridOldPhi(grid.GetDimX(),grid.GetDimY(),grid.GetDimZ()) ;
  LevelSetGrid::Iterator iter = GetGrid().BeginNarrowBand();
  LevelSetGrid::Iterator iend = GetGrid().EndNarrowBand();
  while (iter != iend) {
    unsigned int i = iter.GetI();
    unsigned int j = iter.GetJ();
    unsigned int k = iter.GetK();

    // Compute rate of change
    float ddt = Evaluate(i,j,k);//Beräkna derivatan i n
	gridOldPhi.SetValue(i,j,k,GetGrid().GetValue(i,j,k));
	float phi_new = GetGrid().GetValue(i,j,k)  + ddt*dt;
    // Compute the next time step and store it in the grid
    grid.SetValue(i,j,k,phi_new);
    iter++;
  }
  GetGrid() = grid;

  //Beräkna nytt n+1 via n och n+2
  iter = GetGrid().BeginNarrowBand();
  iend = GetGrid().EndNarrowBand();
  while (iter != iend) {
    unsigned int i = iter.GetI();
    unsigned int j = iter.GetJ();
    unsigned int k = iter.GetK();

    // Compute rate of change
    float ddt = Evaluate(i,j,k); //Beräkna derivatan i n+1

	float old_phi = gridOldPhi.GetValue(i,j,k);
	float phi_new = 0.5*(old_phi +  (GetGrid().GetValue(i,j,k) + ddt*dt));//0.5(n + (n+2)) = n+1
    // Compute the next time step and store it in the grid
    grid.SetValue(i,j,k,phi_new);
    iter++;
  }
  GetGrid() = grid;*/
	/*
	//K1
	LevelSetGrid grid = GetGrid();
	LevelSetGrid y(grid.GetDimX(),grid.GetDimY(),grid.GetDimZ()) ;
	LevelSetGrid k1(grid.GetDimX(),grid.GetDimY(),grid.GetDimZ()) ;
	LevelSetGrid::Iterator iter = GetGrid().BeginNarrowBand();
	LevelSetGrid::Iterator iend = GetGrid().EndNarrowBand();
	while (iter != iend) 
	{
		unsigned int i = iter.GetI();
		unsigned int j = iter.GetJ();
		unsigned int k = iter.GetK();

		// Compute rate of change
		float ddt = Evaluate(i,j,k);//Beräkna derivatan i k1
		y.SetValue(i,j,k,GetGrid().GetValue(i,j,k));
		k1.SetValue(i,j,k,ddt*dt);
		// Compute the next time step and store it in the grid
		grid.SetValue(i,j,k, y.GetValue(i,j,k) + (1.0/2.0)*k1.GetValue(i,j,k));
		iter++;
	}
	GetGrid() = grid;

	//K2
	LevelSetGrid k2(grid.GetDimX(),grid.GetDimY(),grid.GetDimZ()) ;
	 iter = GetGrid().BeginNarrowBand();
	 iend = GetGrid().EndNarrowBand();
	while (iter != iend) 
	{
		unsigned int i = iter.GetI();
		unsigned int j = iter.GetJ();
		unsigned int k = iter.GetK();

		// Compute rate of change
		float ddt = Evaluate(i,j,k);//Beräkna derivatan i k2
		k2.SetValue(i,j,k,ddt*dt);
		// Compute the next time step and store it in the grid
		grid.SetValue(i,j,k, y.GetValue(i,j,k) + (1.0/2.0)*k2.GetValue(i,j,k));
		iter++;
	}
	GetGrid() = grid;

	//K3
	LevelSetGrid k3(grid.GetDimX(),grid.GetDimY(),grid.GetDimZ()) ;
	 iter = GetGrid().BeginNarrowBand();
	 iend = GetGrid().EndNarrowBand();
	while (iter != iend) 
	{
		unsigned int i = iter.GetI();
		unsigned int j = iter.GetJ();
		unsigned int k = iter.GetK();

		// Compute rate of change
		float ddt = Evaluate(i,j,k);//Beräkna derivatan i k3
		k3.SetValue(i,j,k,ddt*dt);
		// Compute the next time step and store it in the grid
		grid.SetValue(i,j,k, y.GetValue(i,j,k) + k3.GetValue(i,j,k));
		iter++;
	}
	GetGrid() = grid;

	//K4
	LevelSetGrid k4(grid.GetDimX(),grid.GetDimY(),grid.GetDimZ()) ;
	 iter = GetGrid().BeginNarrowBand();
	 iend = GetGrid().EndNarrowBand();
	while (iter != iend) 
	{
		unsigned int i = iter.GetI();
		unsigned int j = iter.GetJ();
		unsigned int k = iter.GetK();

		// Compute rate of change
		float ddt = Evaluate(i,j,k);//Beräkna derivatan i k3
		k4.SetValue(i,j,k,ddt*dt);
		iter++;
	}
	GetGrid() = grid;

  //Beräkna nytt n+1 via n och n+2
  iter = GetGrid().BeginNarrowBand();
  iend = GetGrid().EndNarrowBand();
  while (iter != iend) {
    unsigned int i = iter.GetI();
    unsigned int j = iter.GetJ();
    unsigned int k = iter.GetK();
	
	float yn =   y.GetValue(i,j,k);
	float k1n =  k1.GetValue(i,j,k);
	float k2n =  k2.GetValue(i,j,k);
	float k3n =  k3.GetValue(i,j,k);
	float k4n =  k4.GetValue(i,j,k);

	float new_y = yn + (1.0/6.0)*(k1n+k4n) + (1.0/3.0)*(k2n + k3n);

    grid.SetValue(i,j,k,new_y);
    iter++;
  }
  GetGrid() = grid;*/

	LevelSetGrid grid = GetGrid();
	LevelSetGrid n(grid.GetDimX(),grid.GetDimY(),grid.GetDimZ()) ;
	LevelSetGrid::Iterator iter = GetGrid().BeginNarrowBand();
	LevelSetGrid::Iterator iend = GetGrid().EndNarrowBand();
	while (iter != iend) {
		unsigned int i = iter.GetI();
		unsigned int j = iter.GetJ();
		unsigned int k = iter.GetK();

		// Compute rate of change
		float ddt = Evaluate(i,j,k);//Beräkna derivatan i n
		n.SetValue(i,j,k,GetGrid().GetValue(i,j,k));
		float n1 = GetGrid().GetValue(i,j,k)  + ddt*dt;
		// Compute the next time step and store it in the grid
		grid.SetValue(i,j,k,n1);
		iter++;
	}
	GetGrid() = grid;

	iter = GetGrid().BeginNarrowBand();
	iend = GetGrid().EndNarrowBand();
	while (iter != iend) {
		unsigned int i = iter.GetI();
		unsigned int j = iter.GetJ();
		unsigned int k = iter.GetK();

		// Compute rate of change
		float ddt = Evaluate(i,j,k);//Beräkna derivatan i n

		float n2 = GetGrid().GetValue(i,j,k)  + ddt*dt;
		float n0 = n.GetValue(i,j,k);
		float n1 = (n0+n2)/2.0;
		// Compute the next time step and store it in the grid
		grid.SetValue(i,j,k,n1);
		iter++;
	}
	GetGrid() = grid;

	iter = GetGrid().BeginNarrowBand();
	iend = GetGrid().EndNarrowBand();
	while (iter != iend) {
		unsigned int i = iter.GetI();
		unsigned int j = iter.GetJ();
		unsigned int k = iter.GetK();

		// Compute rate of change
		float ddt = Evaluate(i,j,k);//Beräkna derivatan i n
		float n2 = GetGrid().GetValue(i,j,k)  + ddt*dt;
		// Compute the next time step and store it in the grid
		grid.SetValue(i,j,k,n2);
		iter++;
	}
	GetGrid() = grid;

	iter = GetGrid().BeginNarrowBand();
	iend = GetGrid().EndNarrowBand();
	while (iter != iend) {
		unsigned int i = iter.GetI();
		unsigned int j = iter.GetJ();
		unsigned int k = iter.GetK();

		// Compute rate of change
		float ddt = Evaluate(i,j,k);//Beräkna derivatan i n
		float n0 = n.GetValue(i,j,k);
		float n2 = GetGrid().GetValue(i,j,k)  + ddt*dt;
		// Compute the next time step and store it in the grid
		float n12 = 3.0/4.0*n0 + 1.0/4.0*n2;
		grid.SetValue(i,j,k,n12);
		iter++;
	}
	GetGrid() = grid;

	iter = GetGrid().BeginNarrowBand();
	iend = GetGrid().EndNarrowBand();
	while (iter != iend) {
		unsigned int i = iter.GetI();
		unsigned int j = iter.GetJ();
		unsigned int k = iter.GetK();

		// Compute rate of change
		float ddt = Evaluate(i,j,k);//Beräkna derivatan i n
		float n0 = n.GetValue(i,j,k);
		float n32 = GetGrid().GetValue(i,j,k)  + ddt*dt;
		// Compute the next time step and store it in the grid
		float n1 = 1.0/3.0*n0 + 2.0/3.0*n32;
		grid.SetValue(i,j,k,n1);
		iter++;
	}
	GetGrid() = grid;
}

