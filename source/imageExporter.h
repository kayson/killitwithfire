#ifndef IMAGEEXPORTER_H
#define IMAGEEXPORTER_H
#include <string.h>

#include "Freeimage.h"

namespace ImageExporter
{

	void saveImage(char* filename, int width, int height);

	bool saveSequence(int *n, int range, int width, int height);

}//End namespace


#endif //IMAGEEXPORTER_H