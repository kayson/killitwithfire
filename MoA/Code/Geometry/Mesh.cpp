/*************************************************************************************************
 *
 * Modeling and animation (TNM079) 2007
 * Code base for lab assignments. Copyright:
 *   Gunnar Johansson (gunnar.johansson@itn.liu.se)
 *   Ken Museth (ken.museth@itn.liu.se)
 *   Michael Bang Nielsen (bang@daimi.au.dk)
 *   Ola Nilsson (ola.nilsson@itn.liu.se)
 *   Andreas Sˆderstrˆm (andreas.soderstrom@itn.liu.se)
 *
 *************************************************************************************************/
#include "Mesh.h"
#include <iostream>

const Mesh::VisualizationMode Mesh::CurvatureVertex = NewVisualizationMode("Vertex curvature");
const Mesh::VisualizationMode Mesh::CurvatureFace = NewVisualizationMode("Face curvature");

float Mesh::Area() const
{
  std::cerr << "Error: area() not implemented for this Mesh" << std::endl;
  return -1;
}

float Mesh::Volume() const
{
  std::cerr << "Error: volume() not implemented for this Mesh" << std::endl;
  return -1;
}

int Mesh::Genus() const
{
  std::cerr << "Error: genus() not implemented for this Mesh" << std::endl;
  return -1;
}

