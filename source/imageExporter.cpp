#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "imageExporter.h"
#ifdef __APPLE__
#include "glfw.h"
#elif defined _WIN32 || defined _WIN64
#include <GL/glfw.h> // Takes care of everything GL-related
#include <GL/freeglut.h> // Takes care of everything GL-related
#endif

int BitsPerPixel = 24;

char* merge(char* folder, char* filename)
{
	char * newArray = new char[std::strlen(folder)+std::strlen(filename)+1];
	std::strcpy(newArray,folder);
    std::strcat(newArray,filename);

	return newArray;
}

FIBITMAP* convertScreenToBitmap(int width, int height)
{
	BYTE* pixels = new BYTE[3 * width * height];
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	FIBITMAP * image = FreeImage_ConvertFromRawBits(pixels, width, height, 3 * width, BitsPerPixel, 0x0000FF, 0xFF0000, 0x00FF00, false);
	return image;
}

void ImageExporter::saveImage(char* filename, int width, int height)
{
	//På windows måste det finnas en mapp för att den ska sparas. 
	//Borde ändras till att be användaren att välja mapp
	char *folder = "../export/";
	char *file = merge(folder, merge(filename, ".png"));

	
	FIBITMAP * bitmap = convertScreenToBitmap(width, height);
	FreeImage_Save(FIF_PNG, bitmap, file, BitsPerPixel);
}

bool ImageExporter::saveSequence(int *n, int range, int width, int height)
{
	if(*n == range)
		return false;

	saveImage(reinterpret_cast<char*>(n), width, height);

	*n++;

}