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

#include "Util/GreenRedColorMap.h"

ColorMapFactory::FactoryRegistration GreenRedColorMap::mFactoryRegistration("Green-Red", new GreenRedColorMap());

GreenRedColorMap::GreenRedColorMap() {
  mColors.push_back(Vector3<float>(0.0, 1.0, 0.0));
  mColors.push_back(Vector3<float>(1.0, 0.0, 0.0));
}
