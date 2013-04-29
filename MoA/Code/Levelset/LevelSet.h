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
#ifndef __levelset_h__
#define __levelset_h__

#include "Geometry/Implicit.h"
#include "Levelset/LevelSetGrid.h"
#include <iostream>

class LevelSet : public Implicit
{
  friend class LevelSetOperator;

protected :

  LevelSetGrid mGrid;
  //! Grid spacing
  float mDx;

public :

  static const VisualizationMode NarrowBand;

  virtual std::list<VisualizationMode> GetVisualizationModes() {
    std::list<VisualizationMode> L = Implicit::GetVisualizationModes();
    L.push_back(NarrowBand);
    return L;
  }

  LevelSet(float dx);
  LevelSet(float dx, const Implicit & impl);
  LevelSet(float dx, const Implicit & impl, const Bbox & box);
  LevelSet(float dx, const Volume<float> &vol);

  virtual ~LevelSet() { }

  virtual const char * GetTypeName() { return typeid(LevelSet).name(); }

  //! Evaluate the implicit at world coordinates x y z
  virtual float GetValue(float x, float y, float z) const;

  //! Calculate the gradient of the implicit at world coordinates x y z
  virtual Vector3<float> GetGradient(float x, float y, float z) const;

  //! calculate the curvature of the implicit at world coordinates x y z
  virtual float GetCurvature(float x, float y, float z) const;

  //! Sets the bounding box in current frame coordinates
  virtual void SetBoundingBox(const Bbox & b);

  //! Set narrow band width (in number of grid points)
  virtual void SetNarrowBandWidth(int width);

  //! Get narrow band width (in number of grid points)
  int GetNarrowBandWidth();

  inline const float GetDx() const { return mDx; }

  //! First order negative differential in x
  float DiffXm(int i, int j, int k) const;
  //! First order positive differential in x
  float DiffXp(int i, int j, int k) const;
  //! Second order central differential in x
  float DiffXpm(int i, int j, int k) const;
  //! Second order second central differential in x
  float Diff2Xpm(int i, int j, int k) const;

  //! First order negative differential in y
  float DiffYm(int i, int j, int k) const;
  //! First order positive differential in y
  float DiffYp(int i, int j, int k) const;
  //! Second order central differential in y
  float DiffYpm(int i, int j, int k) const;
  //! Second order second central differential in y
  float Diff2Ypm(int i, int j, int k) const;

  //! First order negative differential in z
  float DiffZm(int i, int j, int k) const;
  //! First order positive differential in z
  float DiffZp(int i, int j, int k) const;
  //! Second order central differential in z
  float DiffZpm(int i, int j, int k) const;
  //! Second order second central differential in z
  float Diff2Zpm(int i, int j, int k) const;

  //! First order mixed differential in x,y
  float Diff2XYpm(int i, int j, int k) const;
  //! First order mixed differential in y,z
  float Diff2YZpm(int i, int j, int k) const;
  //! First order mixed differential in z,x
  float Diff2ZXpm(int i, int j, int k) const;

  float WENO(float v1, float v2, float v3, float v4, float v5) const;

  friend std::ostream& operator << (std::ostream &os, const LevelSet &LS)
  {
    os << "Level set bounding box: " << LS.mBox.pMin << " -> " << LS.mBox.pMax << std::endl;
    os << "Level set dx: " << LS.mDx << std::endl;
    os << LS.mGrid << std::endl;
    return os;
  }

  virtual void Render();

  void TransformWorldToGrid(float & i, float & j, float & k) const;

  void TransformGridToWorld(float & x, float & y, float & z) const;

};


#endif
