
#include "Grid.h"

#ifdef __APPLE__
#include "glfw.h"
#elif defined _WIN32 || defined _WIN64
#include <GL/glfw.h>
#endif

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
//Ritar endast ut rutnät för 2D atm
void Grid::draw() const
{
	drawFilled2d();

	drawLines2d();
	
}