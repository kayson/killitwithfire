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

#include "Util/HSVColorMap.h"

ColorMapFactory::FactoryRegistration HSVColorMap::mFactoryRegistration("HSV", new HSVColorMap());

HSVColorMap::HSVColorMap() {
    mColors.push_back(Vector3<float>(1.000000, 0.000000, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.093750, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.187500, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.281250, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.375000, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.468750, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.562500, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.656250, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.750000, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.843750, 0.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.937500, 0.000000));
    mColors.push_back(Vector3<float>(0.968750, 1.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.875000, 1.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.781250, 1.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.687500, 1.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.593750, 1.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.500000, 1.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.406250, 1.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.312500, 1.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.218750, 1.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.125000, 1.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.031250, 1.000000, 0.000000));
    mColors.push_back(Vector3<float>(0.000000, 1.000000, 0.062500));
    mColors.push_back(Vector3<float>(0.000000, 1.000000, 0.156250));
    mColors.push_back(Vector3<float>(0.000000, 1.000000, 0.250000));
    mColors.push_back(Vector3<float>(0.000000, 1.000000, 0.343750));
    mColors.push_back(Vector3<float>(0.000000, 1.000000, 0.437500));
    mColors.push_back(Vector3<float>(0.000000, 1.000000, 0.531250));
    mColors.push_back(Vector3<float>(0.000000, 1.000000, 0.625000));
    mColors.push_back(Vector3<float>(0.000000, 1.000000, 0.718750));
    mColors.push_back(Vector3<float>(0.000000, 1.000000, 0.812500));
    mColors.push_back(Vector3<float>(0.000000, 1.000000, 0.906250));
    mColors.push_back(Vector3<float>(0.000000, 1.000000, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.906250, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.812500, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.718750, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.625000, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.531250, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.437500, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.343750, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.250000, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.156250, 1.000000));
    mColors.push_back(Vector3<float>(0.000000, 0.062500, 1.000000));
    mColors.push_back(Vector3<float>(0.031250, 0.000000, 1.000000));
    mColors.push_back(Vector3<float>(0.125000, 0.000000, 1.000000));
    mColors.push_back(Vector3<float>(0.218750, 0.000000, 1.000000));
    mColors.push_back(Vector3<float>(0.312500, 0.000000, 1.000000));
    mColors.push_back(Vector3<float>(0.406250, 0.000000, 1.000000));
    mColors.push_back(Vector3<float>(0.500000, 0.000000, 1.000000));
    mColors.push_back(Vector3<float>(0.593750, 0.000000, 1.000000));
    mColors.push_back(Vector3<float>(0.687500, 0.000000, 1.000000));
    mColors.push_back(Vector3<float>(0.781250, 0.000000, 1.000000));
    mColors.push_back(Vector3<float>(0.875000, 0.000000, 1.000000));
    mColors.push_back(Vector3<float>(0.968750, 0.000000, 1.000000));
    mColors.push_back(Vector3<float>(1.000000, 0.000000, 0.937500));
    mColors.push_back(Vector3<float>(1.000000, 0.000000, 0.843750));
    mColors.push_back(Vector3<float>(1.000000, 0.000000, 0.750000));
    mColors.push_back(Vector3<float>(1.000000, 0.000000, 0.656250));
    mColors.push_back(Vector3<float>(1.000000, 0.000000, 0.562500));
    mColors.push_back(Vector3<float>(1.000000, 0.000000, 0.468750));
    mColors.push_back(Vector3<float>(1.000000, 0.000000, 0.375000));
    mColors.push_back(Vector3<float>(1.000000, 0.000000, 0.281250));
    mColors.push_back(Vector3<float>(1.000000, 0.000000, 0.187500));
    mColors.push_back(Vector3<float>(1.000000, 0.000000, 0.093750));
}
