#ifndef IMAGEEXPORTER_H
#define IMAGEEXPORTER_H

#include <string.h>

#ifdef __APPLE__
#elif defined _WIN32 || defined _WIN64
#include "Freeimage.h"

//Stäng av freeimage med denna istället för undre raden.
//#undef FREEIMAGE

#define FREEIMAGE

#endif

namespace ImageExporter
{
    static float fps = 30;
    static float timeTrack = 0;
	void saveImage(char filename[], int width, int height);

	bool saveSequence(int &n, float dt, int range, int width, int height);

}//End namespace

#endif //IMAGEEXPORTER_H