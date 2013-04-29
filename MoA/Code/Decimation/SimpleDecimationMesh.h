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
#ifndef _SIMPLE_DECIMATION_MESH
#define _SIMPLE_DECIMATION_MESH

#include "Decimation/DecimationMesh.h"

class SimpleDecimationMesh : public virtual DecimationMesh
{
public :
  SimpleDecimationMesh() { }
  virtual ~SimpleDecimationMesh() { }


protected :

  virtual void computeCollapse(EdgeCollapse * collapse);
};

#endif
