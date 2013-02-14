#include "Grid.h"
#include "Interpolation.h"

#ifdef __APPLE__
#include "firePresets.h"
#elif defined _WIN32 || defined _WIN64
#include "presets\firePresets.h"
#endif

const float RENDERSCALE = 0.4;

Grid::Grid(const int x, const int y, const int z)
{
	xDim = x;
	yDim = y;
	zDim = z;

	grid = new double**[xDim];
	for(int i = 0; i < xDim; i++)
	{
		grid[i] = new double*[yDim];
		for(int j = 0; j < yDim; j++)
			grid[i][j] = new double[zDim];
	}
}

Grid::~Grid()
{
	for(int i = 0; i < xDim; i++)
	{
		for(int j = 0; j < yDim; j++)
			delete [] grid[i][j];

		delete [] grid[i];
	}
}

void Grid::draw() const
{
	drawValue2d();
	//drawLines2d();
}

void Grid::setData(int x, int y, int z, double value)
{ 
	grid[x][y][z] = value; 
};
	
double& Grid::operator()(const int x, const int y, const int z)
{
	//std::cout << "Grid operator(), (x,y,z) = ("<<x<<", "<<y<<", "<<z<<")\n";
	if(x < 0 || y < 0 || z < 0) //Bör nog extrapolera här
		throw;
	if(x >= xDim || y >= yDim || z >= zDim) //Bör nog extrapolera här
		throw;

	return grid[x][y][z];
};

int Grid::getDimX() const 
{ 
	return xDim; 
};

int Grid::getDimY() const 
{ 
	return yDim; 
};

int Grid::getDimZ() const 
{ 
	return zDim; 
};

double Grid::getMax()
{
	double max = grid[0][0][0];
	for(int i = 1; i < xDim; i++)
	{
		for(int j = 0; j < yDim; j++)
		{
			for(int k = 0; k < zDim; k++)
			{
				if (grid[i][j][k] > max)
				{
					max = grid[i][j][k];
				}
			}
		}
	}

	return max;
}

Vector3 Grid::getCellVelocity(Grid &u, Grid &v, Grid &w, const int i, const int j, const int k)
{
	Vector3 x(
		(u(i,j,k) + u(i+1,j,k))/2,
		(v(i,j,k) + v(i,j+1,k))/2,
		(w(i,j,k) + w(i,j,k+1))/2);
	
	return x;
}

Vector3 getGridVelocity(Grid &u, Grid &v, Grid &w, const int i, const int j, const int k, DirectionEnums dir)
{
	if ( dir == UP )
	{
		Vector3 x(
		(u(i,j,k) + u(i+1,j,k) + u(i,j+1,k) + u(i+1,j+1,k))/4,
		v(i,j+1,k),
		(w(i,j,k) + w(i,j,k+1) + w(i,j+1,k) + w(i,j+1,k+1))/4);
	
		return x;
	}
	if ( dir == DOWN )
	{
		Vector3 x(
		(u(i,j,k) + u(i+1,j,k) + u(i,j-1,k) + u(i+1,j-1,k))/4,
		v(i,j,k),
		(w(i,j,k) + w(i,j,k+1) + w(i,j-1,k) + w(i,j-1,k+1))/4);
	
		return x;
	}
	if ( dir == LEFT )
	{
		Vector3 x(
		u(i,j,k),
		(v(i,j,k) + v(i,j+1,k) + v(i-1,j,k) + v(i-1,j+1,k))/4,
		(w(i,j,k) + w(i,j,k+1) + w(i-1,j,k) + w(i-1,j,k+1))/4);
	
		return x;
	}
	if ( dir == RIGHT )
	{
		Vector3 x(
		u(i+1,j,k),
		(v(i,j,k) + v(i,j+1,k) + v(i+1,j,k) + v(i+1,j+1,k))/4,
		(w(i,j,k) + w(i,j,k+1) + w(i+1,j,k) + w(i+1,j,k+1))/4);
	
		return x;
	}
	if ( dir == FORWARD )
	{
		Vector3 x(
		(u(i,j,k) + u(i+1,j,k) + u(i,j,k+1) + u(i+1,j,k+1))/4,
		(v(i,j,k) + v(i,j+1,k) + v(i,j,k+1) + v(i,j+1,k+1))/4,
		w(i,j,k+1));
	
		return x;
	}
	if ( dir == BACKWARD )
	{
		Vector3 x(
		(u(i,j,k) + u(i+1,j,k) + u(i,j,k-1) + u(i+1,j,k-1))/4,
		(v(i,j,k) + v(i,j+1,k) + v(i,j,k-1) + v(i,j+1,k-1))/4,
		w(i,j,k));
	
		return x;
	}
    
    return Vector3(0,0,0);
}

void Grid::drawLines2d() const
{
    double dx = FirePresets::dx;

	glBegin(GL_LINES);
	glColor3f(1,1,1);
    glScaled(dx, dx, dx);

	for(int x = 0; x < xDim-1; x++)
	{
		for(int y = 0; y < yDim-1; y++)
		{
			for(int z = 0; z < zDim; z++)
			{


				glVertex3f(dx*(float)x, dx*(float)y, dx*(float)z);
				glVertex3f(dx*((float)x+1.f), dx*(float)y, dx*(float)z);

				glVertex3f(dx*(float)x, dx*(float)y, dx*(float)z);
				glVertex3f(dx*(float)x, dx*((float)y+1.f), dx*(float)z);

				glVertex3f(dx*((float)x), dx*((float)y+1.f), dx*(float)z);
				glVertex3f(dx*((float)x+1.f), dx*((float)y+1.f), dx*(float)z);

				glVertex3f(dx*((float)x+1.f), dx*(float)y, dx*(float)z);
				glVertex3f(dx*((float)x+1.f), dx*((float)y+1.f), dx*(float)z);
			}
		}
	}
	glEnd();
}

void Grid::drawFilled2d() const
{
    double dx = FirePresets::dx;
	for(int x = 0; x < xDim-1; x++)
	{
		for(int y = 0; y < yDim-1; y++)
		{
			for(int z = 0; z < zDim; z++)
			{

				if(grid[x][y][z] <= 0)
				{
                    
                    glBegin(GL_QUADS);
                    glColor3f(1,0,0);
                    
					glVertex3f(dx*(float)x, dx*(float)y, dx*(float)z);
					glVertex3f(dx*((float)x+1.f), dx*((float)y), dx*((float)z));
					glVertex3f(dx*((float)x+1.f), dx*((float)y+1.f), dx*(float)z);
					glVertex3f(dx*((float)x), dx*((float)y+1.f), dx*((float)z));
                    glEnd();

				}
			}
		}
	}
}

void Grid::drawValue2d() const
{
    double dx = FirePresets::dx;
	for(int x = 0; x < xDim-1; x++)
	{
		for(int y = 0; y < yDim-1; y++)
		{
			for(int z = 0; z < zDim; z++)
			{
                glBegin(GL_QUADS);

				if(grid[x][y][z] <= 0)
				{
                    glColor3f(0,0,-grid[x][y][z]/8.0);
                    //glColor3f(0,0,1);
                
                    
				}else{
                    glColor3f(grid[x][y][z]/100.0,0,0);
                   // glColor3f(1,0,0);
                
                }
                
                glVertex3f(dx*(float)x, dx*(float)y, dx*(float)z);
                glVertex3f(dx*((float)x+1.f), dx*(float)y, dx*(float)z);
                glVertex3f(dx*((float)x+1.f), dx*((float)y+1.f), dx*(float)z);
                glVertex3f(dx*(float)x, dx*((float)y+1.f), dx*(float)z);
                
                glEnd();

			}
		}
	}
}


