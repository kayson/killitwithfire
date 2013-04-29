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

#include "Util/JetColorMap.h"

ColorMapFactory::FactoryRegistration JetColorMap::mFactoryRegistration("Jet", new JetColorMap());

JetColorMap::JetColorMap() {
    mColors.push_back(Vector3<float>(0.000000, 0.000000, 0.562500));
    mColors.push_back(Vector3<float>(0.000000, 0.000000, 0.625000));
    mColors.push_back(Vector3<float>(0.000000, 0.000000, 0.687500));
    mColors.push_back(Vector3<float>(0.000000, 0.000000, 0.750000));
    mColors.push_back(Vector3<float>(0.000000, 0.000000, 0.812500));
    mColors.push_back(Vector3<float>(0.000000, 0.000000, 0.875000));
    mColors.push_back(Vector3<float>(0.000000, 0.000000, 0.937500));
    mColors.push_back(Vector3<float>(0.000000, 0.000000, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.062500, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.125000, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.187500, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.250000, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.312500, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.375000, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.437500, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.500000, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.562500, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.625000, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.687500, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.750000, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.812500, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.875000, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.937500, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 1.000000, 1.000000));
    mColors.push_back(Vector3<float>(0.062500, 1.000000, 0.937500));
    mColors.push_back(Vector3<float>(0.125000, 1.000000, 0.875000));
    mColors.push_back(Vector3<float>(0.187500, 1.000000, 0.812500));
    mColors.push_back(Vector3<float>(0.250000, 1.000000, 0.750000));
    mColors.push_back(Vector3<float>(0.312500, 1.000000, 0.687500));
    mColors.push_back(Vector3<float>(0.375000, 1.000000, 0.625000));
    mColors.push_back(Vector3<float>(0.437500, 1.000000, 0.562500));
    mColors.push_back(Vector3<float>(0.500000, 1.000000, 0.500000));
    mColors.push_back(Vector3<float>(0.562500, 1.000000, 0.437500));
    mColors.push_back(Vector3<float>(0.625000, 1.000000, 0.375000));
    mColors.push_back(Vector3<float>(0.687500, 1.000000, 0.312500));
    mColors.push_back(Vector3<float>(0.750000, 1.000000, 0.250000));
    mColors.push_back(Vector3<float>(0.812500, 1.000000, 0.187500));
    mColors.push_back(Vector3<float>(0.875000, 1.000000, 0.125000));
    mColors.push_back(Vector3<float>(0.937500, 1.000000, 0.062500));
    mColors.push_back(Vector3<float>(1.000000, 1.000000, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.937500, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.875000, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.812500, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.750000, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.687500, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.625000, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.562500, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.500000, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.437500, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.375000, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.312500, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.250000, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.187500, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.125000, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.062500, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.937500, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.875000, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.812500, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.750000, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.687500, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.625000, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.562500, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.500000, 0.000000, 0.000000));
}
