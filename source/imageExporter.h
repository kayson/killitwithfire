#ifndef IMAGEEXPORTER_H
#define IMAGEEXPORTER_H
#include <string.h>


#ifdef __APPLE__
#elif defined _WIN32 || defined _WIN64
#include "Freeimage.h"
#endif

namespace ImageExporter
{

	void saveImage(char* filename, int width, int height);

	bool saveSequence(int *n, int range, int width, int height);

}//End namespace


#endif //IMAGEEXPORTER_H