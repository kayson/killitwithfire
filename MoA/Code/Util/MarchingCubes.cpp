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
#include "MarchingCubes.h"

/*!
 * Grabbed from:
 * http://astronomy.swin.edu.au/~pbourke/modelling/polygonise/
 * Modified slightly.
 *
 * NB! Uses clockwise orientation.
 */
std::vector<Vector3<float> > Triangulate(float voxelValues[8], float i, float j, float k, float delta) {
#include "MarchingCubesTable.h"
  int cubeindex = 0;
  static Vector3<float> vertlist[12];
  std::vector<Vector3<float> > verts;

  if (voxelValues[0] < 0.f) cubeindex |= 1;
  if (voxelValues[1] < 0.f) cubeindex |= 2;
  if (voxelValues[2] < 0.f) cubeindex |= 4;
  if (voxelValues[3] < 0.f) cubeindex |= 8;
  if (voxelValues[4] < 0.f) cubeindex |= 16;
  if (voxelValues[5] < 0.f) cubeindex |= 32;
  if (voxelValues[6] < 0.f) cubeindex |= 64;
  if (voxelValues[7] < 0.f) cubeindex |= 128;

  /* Cube is entirely in/out of the surface */
  if (edgeTable[cubeindex] == 0)
    return verts;

  /* Find the vertices where the surface intersects the cube */
  if (edgeTable[cubeindex] & 1)
    // 0-1
    vertlist[0] = Vector3<float>(i + Root(voxelValues[0], voxelValues[1])*delta, j, k);
  if (edgeTable[cubeindex] & 2)
    // 1-2
    vertlist[1] = Vector3<float>(i+delta, j + Root(voxelValues[1], voxelValues[2])*delta, k);
  if (edgeTable[cubeindex] & 4)
    // 3-2
    vertlist[2] = Vector3<float>(i + Root(voxelValues[3], voxelValues[2])*delta, j+delta, k);
  if (edgeTable[cubeindex] & 8)
    // 0-3
    vertlist[3] = Vector3<float>(i, j + Root(voxelValues[0], voxelValues[3])*delta, k);
  if (edgeTable[cubeindex] & 16)
    //  4-5
    vertlist[4] = Vector3<float>(i + Root(voxelValues[4], voxelValues[5])*delta, j, k+delta);
  if (edgeTable[cubeindex] & 32)
    //  5-6
    vertlist[5] = Vector3<float>(i+delta, j + Root(voxelValues[5], voxelValues[6])*delta, k+delta);
  if (edgeTable[cubeindex] & 64)
    //  7-6
    vertlist[6] = Vector3<float>(i + Root(voxelValues[7], voxelValues[6])*delta, j+delta, k+delta);
  if (edgeTable[cubeindex] & 128)
    //  4-7
    vertlist[7] = Vector3<float>(i, j + Root(voxelValues[4], voxelValues[7])*delta, k+delta);
  if (edgeTable[cubeindex] & 256)
    // 0-4
    vertlist[8] = Vector3<float>(i, j, k + Root(voxelValues[0], voxelValues[4])*delta);
  if (edgeTable[cubeindex] & 512)
    // 1-5
    vertlist[9] = Vector3<float>(i+delta, j  , k + Root(voxelValues[1], voxelValues[5])*delta);
  if (edgeTable[cubeindex] & 1024)
    // 2-6
    vertlist[10] = Vector3<float>(i+delta, j+delta, k + Root(voxelValues[2], voxelValues[6])*delta);
  if (edgeTable[cubeindex] & 2048)
    // 3-7
    vertlist[11] = Vector3<float>(i  , j+delta, k + Root(voxelValues[3], voxelValues[7])*delta);

  /* Create the triangle */
  for (unsigned int m=0;triTable[cubeindex][m]!=-1;m+=3) {
    // Use counter clockwise orientation
    verts.push_back(vertlist[triTable[cubeindex][m  ]]);
    verts.push_back(vertlist[triTable[cubeindex][m+2]]);
    verts.push_back(vertlist[triTable[cubeindex][m+1]]);
  }

  return verts;
}
