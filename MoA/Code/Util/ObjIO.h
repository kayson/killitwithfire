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
#ifndef __obj_io_h__
#define __obj_io_h__

#include "Geometry/Mesh.h"
#include "Math/Vector3.h"
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

class ObjIO {
public:
  ObjIO() {}

  bool Load(Mesh *, std::istream & is); // false return on error

protected:
  bool ReadHeader(std::istream &is);
  bool ReadData(std::istream &is);

  static Vector3<unsigned int> ReadTri(std::istream &is);

  struct LoadData{
    std::vector<Vector3<float> > verts;
    std::vector<Vector3<unsigned int> > tris;
  } loadData;
};

#endif
