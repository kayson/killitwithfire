#ifndef GRID_H
#define GRID_H

#include "datatype.h"
#include <vector>

#ifdef __APPLE__
#include "glfw.h"
#elif defined _WIN32 || defined _WIN64
#include <GL/glfw.h>
#endif


template <class T>
class Grid
{
public:
	Grid(const int x = GRID_DIM_X, const int y = GRID_DIM_Y, const int z = GRID_DIM_Z)
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

	~Grid()
	{
		for(int i = 0; i < xDim; i++)
		{
			for(int j = 0; j < yDim; j++)
				delete [] grid[i][j];

			delete [] grid[i];
		}
	}

	void draw() const
	{
		drawFilled2d();

		drawLines2d();
	}

	//Setters
	void setData(int x, int y, int z, T value){ grid[x][y][z] = value; };
	
	T& operator()(const int x, const int y, const int z)
	{
		//if(x < xDim && y < yDim && z < zDim)
			//Skicka exception annars istället? dvs
		//if(x >= xDim || y >= yDim || z >= zDim)
			//throw; 	

		return grid[x][y][z];
	};

	//Getters
	int getDimX() const { return xDim; };
	int getDimY() const { return yDim; };
	int getDimZ() const { return zDim; };

private:
	void drawLines2d() const
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

	void drawFilled2d() const
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

	T ***grid;
	int xDim, yDim, zDim;
};

#endif
