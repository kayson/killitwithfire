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
#ifndef __util_h__
#define __util_h__

#include <string> // for string
#include <algorithm> //for transform
#include <cmath>

//! \brief Convert a string to lowercase
std::string& Lower(std::string & str);
//! Construct a new string without whitespace at start or end
std::string Trim(std::string & str);


/*! Stable computation of cotangents */
template<class VectorT>
typename VectorT::ValueType Cotangent(const VectorT& a, const VectorT& b, const VectorT& c){
  const VectorT ba = a - b;
  const VectorT bc = c - b;

  return bc*ba / Cross(bc, ba).Length();
}

/*! \brief Return the sign of a value
 *
 *  \f{eqnarray*}
 *  \begin{cases} -1& x<0\\ 1& x\geq 0 \end{cases}
 *  \f}
 *
 * \param[in] value the value to get the sign of
 * \return the sign as a T
 */
template<typename Scalar>
inline Scalar Sign(Scalar value)
{
  Scalar mysign = -(Scalar)1.0;
  if (value >= (Scalar)0.0){
    mysign = (Scalar)1.0;
  }
  return mysign;
}

/*! \brief Clamp val to [low, high]
 *
 * \param[in] val the value to clamp
 * \param[in] low the lowest return value
 * \param[in] high the highest return value
 * \return the clamped value as a Scalar
 */
template <typename Scalar>
inline Scalar Clamp(Scalar val, Scalar low, Scalar high)
{
  if (val < low) return low;
  else if (val > high) return high;
  else return val;
}

/*! \brief First order switch function
 *
 * Maps the input data [x1, x2] to [0,1]. Outside values are clamped to fit in [0,1].
 * \param[in] val the value to switch
 * \param[in] x1 the lower map border
 * \param[in] x2 the higher map border
 * \return a value in [0,1] as a Scalar
 */
template <class Scalar>
inline Scalar Switch1(Scalar val, Scalar x1, Scalar x2){
  return ( val<=x1 ? 0 : ( val>=x2 ? 1 : (val-x1)/(x2-x1)) );
}

/*! \brief First order root finding
 *
 *  \f{eqnarray*}
 *  f(0) = y_0 \\
 *  f(1) = y_1 \\
 *  f(x) = kx + m\\
 *  f(x) = 0, \quad \rightarrow \quad x = -m/k
 *  \f}

 * \param[in] y0 the value of \f$f(0)\f$
 * \param[in] y1 the value of \f$f(1)\f$
 * \return the value for which \f$f(x)=0\f$
 */
inline float Root(float y0, float y1){
  const float m = y0;
  const float k = y1-y0;
  return -m/k;
}


inline float Round(float d)
{
  return std::floor(d + 0.5);
}

//! Test to see if host has big endian byte order
inline bool IsBigEndian()
{
  short int word = 0x001;
  char *byte = (char *) &word;
  return (byte[0] ? false : true);
}

//! Byte swap 32bit uint
inline unsigned int EndianSwap(unsigned int x)
{
  return ((x>>24) |
          ((x<<8) & 0x00FF0000) |
          ((x>>8) & 0x0000FF00) |
          (x<<24));
}

//! Byte swap array, only works for 32bit types
template<typename T>
inline void EndianSwap(T* data, int num){
  if(sizeof(T) == sizeof(unsigned int)){
    unsigned int * tmpPtr = reinterpret_cast<unsigned int *>(data);
    std::transform(tmpPtr, tmpPtr+num,
                   tmpPtr,
                   std::pointer_to_unary_function<unsigned int, unsigned int>(&EndianSwap));
  } else {
    // badness
  }
}




#endif
