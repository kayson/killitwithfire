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

#include "Util/HotColorMap.h"

ColorMapFactory::FactoryRegistration HotColorMap::mFactoryRegistration("Hot", new HotColorMap());

HotColorMap::HotColorMap() {
    mColors.push_back(Vector3<float>(0.041667, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.083333, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.125000, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.166667, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.208333, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.250000, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.291667, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.333333, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.375000, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.416667, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.458333, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.500000, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.541667, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.583333, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.625000, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.666667, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.708333, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.750000, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.791667, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.833333, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.875000, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.916667, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.958333, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.041667, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.083333, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.125000, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.166667, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.208333, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.250000, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.291667, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.333333, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.375000, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.416667, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.458333, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.500000, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.541667, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.583333, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.625000, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.666667, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.708333, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.750000, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.791667, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.833333, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.875000, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.916667, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.958333, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 1.000000, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 1.000000, 0.062500));
    mColors.push_back(Vector3<float>(1.000000, 1.000000, 0.125000));
    mColors.push_back(Vector3<float>(1.000000, 1.000000, 0.187500));
    mColors.push_back(Vector3<float>(1.000000, 1.000000, 0.250000));
    mColors.push_back(Vector3<float>(1.000000, 1.000000, 0.312500));
    mColors.push_back(Vector3<float>(1.000000, 1.000000, 0.375000));
    mColors.push_back(Vector3<float>(1.000000, 1.000000, 0.437500));
    mColors.push_back(Vector3<float>(1.000000, 1.000000, 0.500000));
    mColors.push_back(Vector3<float>(1.000000, 1.000000, 0.562500));
    mColors.push_back(Vector3<float>(1.000000, 1.000000, 0.625000));
    mColors.push_back(Vector3<float>(1.000000, 1.000000, 0.687500));
    mColors.push_back(Vector3<float>(1.000000, 1.000000, 0.750000));
    mColors.push_back(Vector3<float>(1.000000, 1.000000, 0.812500));
    mColors.push_back(Vector3<float>(1.000000, 1.000000, 0.875000));
    mColors.push_back(Vector3<float>(1.000000, 1.000000, 0.937500));
    mColors.push_back(Vector3<float>(1.000000, 1.000000, 1.000000));
}
