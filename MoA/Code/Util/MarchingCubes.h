/*************************************************************************************************
 *
 * Modeling and animation (TNM079) 2007
 * Code base for lab assignments. Copyright:
 *   Gunnar Johansson (gunnar.johansson@itn.liu.se)
 *   Ken Museth (ken.museth@itn.liu.se)
 *   Michael Bang Nielsen (bang@daimi.au.dk)
 *   Ola Nilsson (ola.nilsson@itn.liu.se)
 *   Andreas Söderström (andreas.soderstrom@itn.liu.se)
 *
 *************************************************************************************************/
#ifndef __marching_cubes_h__
#define __marching_cubes_h__

#include <vector>
#include "Math/Vector3.h"
#include "Util/Util.h"

//! Method to triangulate a voxel
std::vector<Vector3<float> > Triangulate(float voxelValues[8], float i, float j, float k, float delta);


#endif
