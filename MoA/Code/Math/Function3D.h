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
#ifndef __function_3d_h__
#define __function_3d_h__

/*! \brief Base class for functions in R3
 * A Function3D can be evaluated anywhere in space.
 */
template<class T>
class Function3D{
public:
  //! Evaluate the function at x,y,z
  virtual T GetValue(float x, float y, float z) const = 0;
  //! Return a bound on the maximum value of the function
  virtual T GetMaxValue() const = 0;
  virtual ~Function3D() {}
};

#endif
