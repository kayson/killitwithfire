/*************************************************************************************************
 *
 * Modeling and animation (TNM079) 2007
 * Code base for lab assignments. Copyright:
 *   Gunnar Johansson (gunnar.johansson@itn.liu.se)
 *   Ken Museth (ken.museth@itn.liu.se)
 *   Michael Bang Nielsen (bang@daimi.au.dk)
 *   Ola Nilsson (ola.nilsson@itn.liu.se)
 *   Andreas Sderstrm (andreas.soderstrom@itn.liu.se)
 *
 *************************************************************************************************/
#ifndef __operatorreinitializefastmarching_h__
#define __operatorreinitializefastmarching_h__

#include "Levelset/LevelSetOperator.h"
#include "Util/Heap.h"
#include <map>
#include <cmath>

/*! \brief A level set operator that re-initializes a level set towards a signed distance function.
 *
 * This class implements level set re-initialization by fast marching
 *
 */
class OperatorReinitializeFastMarching : public LevelSetOperator
{
protected :

  struct Coordinate
  {
    Coordinate(int I, int J, int K) : i(I), j(J), k(K), inside(false) { }
    Coordinate(int I, int J, int K, bool Inside) : i(I), j(J), k(K), inside(Inside) { }
    int i, j, k;
    bool inside;

    bool operator < (const Coordinate & coord) const {
      return                  i < coord.i  ||
      (i == coord.i && j < coord.j) ||
      (i == coord.i && j == coord.j && k < coord.k);
    }
  };

  class Node : public Heap::Heapable
  {
    public :
    Node(Coordinate coord) : coordinate(coord) { }
    Node(int i, int j, int k) : coordinate(i,j,k) { }
    Coordinate coordinate;
  };


  float ComputeTentativeValue(int i, int j, int k)
  {
    const float inf = std::numeric_limits<float>::max();
    std::map<Coordinate, float>::const_iterator iter;

    // Fetch (i-1,j,k)   (returns infinity if not accepted)
    iter = mAccepted.find(Coordinate(i-1,j,k));
    float im = (iter == mAccepted.end() ? inf : (*iter).second);

    // Fetch (i+1,j,k)
    iter = mAccepted.find(Coordinate(i+1,j,k));
    float ip = (iter == mAccepted.end() ? inf : (*iter).second);

    // Fetch (i,j-1,k)
    iter = mAccepted.find(Coordinate(i,j-1,k));
    float jm = (iter == mAccepted.end() ? inf : (*iter).second);

    // Fetch (i,j+1,k)
    iter = mAccepted.find(Coordinate(i,j+1,k));
    float jp = (iter == mAccepted.end() ? inf : (*iter).second);

    // Fetch (i,j,k-1)
    iter = mAccepted.find(Coordinate(i,j,k-1));
    float km = (iter == mAccepted.end() ? inf : (*iter).second);

    // Fetch (i,j,k+1)
    iter = mAccepted.find(Coordinate(i,j,k+1));
    float kp = (iter == mAccepted.end() ? inf : (*iter).second);

    // Get the minimum along each dimension
    float f1 = std::min(im, ip);
    float f2 = std::min(jm, jp);
    float f3 = std::min(km, kp);

    // Sort the values in ascending order
    if (f1 > f2)  std::swap(f1,f2);
    if (f2 > f3)  std::swap(f2,f3);
    if (f1 > f2)  std::swap(f1,f2);

    assert(f1 != inf);

    // Case 1: We have accepted values only along one dimension
    if (f2 == inf)  return f1 + 1;

    // Case 2: We have accepted values along two dimensions
    if (f3 == inf) {
      float b = f1 + f2;
      float root = b*b - 2*(f1*f1 + f2*f2 - 1);
      if (root < 0)  root = 0;
      return (b + std::sqrt(root))*0.5;
    }

    // Case 3: We have accepted values along all three dimensions
    float b = f1 + f2 + f3;
    float root = b*b - 3*(f1*f1 + f2*f2 + f3*f3 - 1);
    if (root < 0)  root = 0;
    return (b + std::sqrt(root)) / 3.0;
  }


  void UpdateTentative(int i, int j, int k, bool inside)
  {
    // Check bounds
    if (i < 0 || i > GetGrid().GetDimX()-1 ||
        j < 0 || j > GetGrid().GetDimY()-1 ||
        k < 0 || k > GetGrid().GetDimZ()-1)
      return;

    Coordinate coord(i,j,k,inside);

    // Exit if already accepted
    if (mAccepted.find(coord) != mAccepted.end())  return;

    // Find the tentative value and see if it's been deleted
    // (it was outside the narrow band)
    std::map<Coordinate, Node *>::iterator iter = mTentative.find(coord);
    if (iter != mTentative.end() && (*iter).second == NULL) return;

    // Otherwise, compute tentative value
    float value = ComputeTentativeValue(i,j,k);
    //mexPrintf("Tentative value for point (%i,%i,%i) computed: %f (bandwidth: %f)\n", i,j,k, value, bandwidth);

    // Check whether we're inside the narrowband
    bool insideNarrowBand =  inside && value*mLS->GetDx() <= -GetGrid().GetInsideConstant() ||
                            !inside && value*mLS->GetDx() <= GetGrid().GetOutsideConstant();

    // The point is new and within the narrowband
    if (iter == mTentative.end() && insideNarrowBand) {
      Node * node = new Node(coord);
      node->cost = value;

      mHeap.push(node);
      mTentative[coord] = node;
      //mexPrintf("Adding (%i,%i,%i) as new tentative point with value %f\n", i,j,k, value);
    }
    // The point is not new and within the narrowband
    else if (insideNarrowBand) {
      Node * node = (*iter).second;
      node->cost = value;

      mHeap.update(node);
      //mexPrintf("Updating tentative point (%i,%i,%i) with value %f\n", i,j,k, value);
    }
    // The point is outside the narrowband and should be deleted
    // if possible
    else if (iter != mTentative.end()) {
      //mexPrintf("Outside narrowband at (%i,%i,%i)\n", i,j,k);
      Node * node = (*iter).second;
      mHeap.remove(node);
      delete node;
      (*iter).second = NULL;
    }
    //mexPrintf(" with value %f\n", value);
  }


  /*
   * Interpolates new distance values for center point given
   * the four neighbors. Assumes center > 0.
   */
  float InterpolateDistance(float center,
                            float im,
                            float ip,
                            float jm,
                            float jp,
                            float km,
                            float kp)
  {
    float b, val = std::numeric_limits<float>::max();
    if (im <= 0) {
      b = center / (center - im);
      if (val > b) val = b;
    }
    if (ip <= 0) {
      b = center / (center - ip);
      if (val > b) val = b;
    }
    if (jm <= 0) {
      b = center / (center - jm);
      if (val > b) val = b;
    }
    if (jp <= 0) {
      b = center / (center - jp);
      if (val > b) val = b;
    }
    if (km <= 0) {
      b = center / (center - km);
      if (val > b) val = b;
    }
    if (kp <= 0) {
      b = center / (center - kp);
      if (val > b) val = b;
    }

    return val;
  }

public :

  OperatorReinitializeFastMarching(LevelSet * LS) : LevelSetOperator(LS) { }

  virtual void Propagate(float time)
  {
    // Report maximum norm of gradient before reinitialization
    std::cerr << "Maximum gradient before reinit: " << MaxNormGradient() << std::endl;

    // Clear maps (we can assume that mHeap is empty, given last loop)
    mAccepted.clear();
    mTentative.clear();

    // Iterate the narrow band and find all points adjacent to
    // the zero-crossing
    LevelSetGrid::Iterator iter = GetGrid().BeginNarrowBand();
    LevelSetGrid::Iterator iend = GetGrid().EndNarrowBand();
    while (iter != iend) {
      unsigned int i = iter.GetI();
      unsigned int j = iter.GetJ();
      unsigned int k = iter.GetK();

      float val = GetGrid().GetValue(i, j, k);
      float im = GetGrid().GetValue(i-1, j,   k);
      float ip = GetGrid().GetValue(i+1, j,   k);
      float jm = GetGrid().GetValue(i,   j-1, k);
      float jp = GetGrid().GetValue(i,   j+1, k);
      float km = GetGrid().GetValue(i,   j,   k-1);
      float kp = GetGrid().GetValue(i,   j,   k+1);

      // If we have a zero-crossing, add the point to the accepted values
      // Flip the sign if we are inside
      if (val > 0 && (im <= 0 || ip <= 0 || jm <= 0 || jp <= 0 || km <= 0 || kp <= 0)) {
        mAccepted[Coordinate(i,j,k)] = InterpolateDistance(val, im, ip, jm, jp, km, kp);
        //mexPrintf("Adding outside (%i,%i,%i) to accepted values (%f)\n", i,j,k, interpolateDistance(val, im, ip, jm, jp, km, kp));
      }
      else if (val <= 0 && (im > 0 || ip > 0 || jm > 0 || jp > 0 || km > 0 || kp > 0)) {
        mAccepted[Coordinate(i,j,k,true)] = InterpolateDistance(-val, -im, -ip, -jm, -jp, -km, -kp);
        //mexPrintf("Adding inside (%i,%i,%i) to accepted values (%f)\n", i,j,k, -interpolateDistance(-val, -im, -ip, -jm, -jp, -km, -kp));
      }

      iter++;
    }

    // Iterate the accepted points and compute tentative distances
    // for all neighbors
    std::map<Coordinate, float>::const_iterator iterAccepted = mAccepted.begin();
    std::map<Coordinate, float>::const_iterator iendAccepted = mAccepted.end();
    while (iterAccepted != iendAccepted) {

      int i = (*iterAccepted).first.i;
      int j = (*iterAccepted).first.j;
      int k = (*iterAccepted).first.k;
      //mexPrintf("Visiting (%i,%i,%i)\n", i,j,k);

      UpdateTentative(i-1,j  ,k  , (*iterAccepted).first.inside);
      UpdateTentative(i+1,j  ,k  , (*iterAccepted).first.inside);
      UpdateTentative(i  ,j-1,k  , (*iterAccepted).first.inside);
      UpdateTentative(i  ,j+1,k  , (*iterAccepted).first.inside);
      UpdateTentative(i  ,j  ,k-1, (*iterAccepted).first.inside);
      UpdateTentative(i  ,j  ,k+1, (*iterAccepted).first.inside);

      iterAccepted++;
    }


    // Accept the smallest tentative value, update its neighbors
    // and continue until we're done
    while (!mHeap.isEmpty()) {
      Node * node = static_cast<Node *>(mHeap.pop());
      int i = node->coordinate.i;
      int j = node->coordinate.j;
      int k = node->coordinate.k;
      //mexPrintf("Accepting (%i,%i) with value %f\n", i, j, node->cost);

      mAccepted[node->coordinate] = node->cost;

      UpdateTentative(i-1,j  ,k  , node->coordinate.inside);
      UpdateTentative(i+1,j  ,k  , node->coordinate.inside);
      UpdateTentative(i  ,j-1,k  , node->coordinate.inside);
      UpdateTentative(i  ,j+1,k  , node->coordinate.inside);
      UpdateTentative(i  ,j  ,k-1, node->coordinate.inside);
      UpdateTentative(i  ,j  ,k+1, node->coordinate.inside);

      delete node;
    }

    // Loop through narrowband and reset the mask and
    // all values to inside/outside constant
    iter = GetGrid().BeginNarrowBand();
    while (iter != iend) {
      unsigned int i = iter.GetI();
      unsigned int j = iter.GetJ();
      unsigned int k = iter.GetK();

      if (GetGrid().GetValue(i,j,k) > 0)
        GetGrid().SetValue(i,j,k, GetGrid().GetOutsideConstant());
      else
        GetGrid().SetValue(i,j,k, GetGrid().GetInsideConstant());

      GetGrid().SetMask(i,j,k, false);
      iter++;
    }

    // Loop through all accepted narrowband points and update grid
    iterAccepted = mAccepted.begin();
    while (iterAccepted != iendAccepted) {

      int i = (*iterAccepted).first.i;
      int j = (*iterAccepted).first.j;
      int k = (*iterAccepted).first.k;

      // Flip sign if we're on the inside
      if ((*iterAccepted).first.inside)
        GetGrid().SetValue(i,j,k, -(*iterAccepted).second * mLS->GetDx());
      else
        GetGrid().SetValue(i,j,k, (*iterAccepted).second * mLS->GetDx());

      iterAccepted++;
    }

    // Report maximum norm of gradient after reinitialization
    std::cerr << "Maximum gradient after reinit: " << MaxNormGradient() << std::endl;
  }




protected :

  // Map of accepted grid points
  std::map<Coordinate, float> mAccepted;

  // Map of tentative grid points
  std::map<Coordinate, Node *> mTentative;

  // Heap used to store and fetch smallest tentative grid points
  Heap mHeap;


  float MaxNormGradient()
  {
    float maxGrad = -(std::numeric_limits<float>::max)();
    LevelSetGrid::Iterator iter = GetGrid().BeginNarrowBand();
    LevelSetGrid::Iterator iend = GetGrid().EndNarrowBand();
    while (iter != iend) {
      unsigned int i = iter.GetI();
      unsigned int j = iter.GetJ();
      unsigned int k = iter.GetK();

      float ddxc = mLS->DiffXpm(i,j,k);
      float ddyc = mLS->DiffYpm(i,j,k);
      float ddzc = mLS->DiffZpm(i,j,k);
      float normgrad2 = ddxc*ddxc + ddyc*ddyc + ddzc*ddzc;
      if (maxGrad < normgrad2) maxGrad = normgrad2;

      iter++;
    }
   return std::sqrt(maxGrad);
  }




};

#endif
