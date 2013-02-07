#include "Grid.h"

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
	//if(x < xDim && y < yDim && z < zDim)
		//Skicka exception annars istället? dvs
	//if(x >= xDim || y >= yDim || z >= zDim)
		//throw; 	

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

Vector3 Grid::getCellVelocity(Grid &u, Grid &v, Grid &w, const int i, const int j, const int k)
{
	Vector3 x(
		(u(i,j,k) + u(i+1,j,k))/2,
		(v(i,j,k) + v(i,j+1,k))/2,
		(w(i,j,k) + w(i,j,k+1))/2);
	
	return x;
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