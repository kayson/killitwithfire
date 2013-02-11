#include "Grid.h"
#include "Interpolation.h"

const float RENDERSCALE = 0.4;

Grid::Grid(const int x, const int y, const int z)
{
	xDim = x;
	yDim = y;
	zDim = z;

	grid = new data**[xDim];
	for(int i = 0; i < xDim; i++)
	{
		grid[i] = new data*[yDim];
		for(int j = 0; j < yDim; j++)
			grid[i][j] = new data[zDim];
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
	drawFilled2d();
	drawLines2d();
}

void Grid::setData(int x, int y, int z, data value)
{ 
	grid[x][y][z] = value; 
};
	
data& Grid::operator()(const int x, const int y, const int z)
{
	if(x < 0 || y < 0 || z || 0) //Bör nog extrapolera här
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
	double max = DBL_MIN;
	for(int i = 0; i < xDim; i++)
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
}

void Grid::drawLines2d() const
{
	
	glBegin(GL_LINES);
	glColor3f(1,1,1);
	for(int x = 0; x < xDim-1; x++)
	{
		for(int y = 0; y < yDim-1; y++)
		{
			for(int z = 0; z < zDim; z++)
			{


				glVertex3f((float)x, (float)y, (float)z);
				glVertex3f((float)x+1.f, (float)y, (float)z);

				glVertex3f((float)x, (float)y, (float)z);
				glVertex3f((float)x, (float)y+1.f, (float)z);

				glVertex3f((float)x, (float)y+1.f, (float)z);
				glVertex3f((float)x+1.f, (float)y+1.f, (float)z);

				glVertex3f((float)x+1.f, (float)y, (float)z);
				glVertex3f((float)x+1.f, (float)y+1.f, (float)z);
			}
		}
	}
	glEnd();
}

void Grid::drawFilled2d() const
{
	glBegin(GL_QUADS);
	glColor3f(1,0,0);
	for(int x = 0; x < xDim-1; x++)
	{
		for(int y = 0; y < yDim-1; y++)
		{
			for(int z = 0; z < zDim; z++)
			{

				if(grid[x][y][z] <= 0)
				{
					glVertex3f((float)x, (float)y, (float)z);
					glVertex3f((float)x+1.f, (float)y, (float)z);
					glVertex3f((float)x+1.f, (float)y+1.f, (float)z);
					glVertex3f((float)x, (float)y+1.f, (float)z);
				}
			}
		}
	}
	glEnd();
}

void Grid::drawValue2d() const
{
	glBegin(GL_QUADS);
	for(int x = 0; x < xDim-1; x++)
	{
		for(int y = 0; y < yDim-1; y++)
		{
			for(int z = 0; z < zDim; z++)
			{
                
				if(grid[x][y][z] <= 0)
				{
                    glColor3f(0,0,-grid[x][y][z]/8.0);
                    
					glVertex3f((float)x, (float)y, (float)z);
					glVertex3f((float)x+1.f, (float)y, (float)z);
					glVertex3f((float)x+1.f, (float)y+1.f, (float)z);
					glVertex3f((float)x, (float)y+1.f, (float)z);
                    
				}else{
                    glColor3f(grid[x][y][z]/100.0,0,0);
                    
					glVertex3f((float)x, (float)y, (float)z);
					glVertex3f((float)x+1.f, (float)y, (float)z);
					glVertex3f((float)x+1.f, (float)y+1.f, (float)z);
					glVertex3f((float)x, (float)y+1.f, (float)z);
                }
			}
		}
	}
	glEnd();
}


