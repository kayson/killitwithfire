/*************************************************************************************************
 *
 *
 *
 *
 *************************************************************************************************
 * Contributors:
 *                  1) IML++
 *                  2) Ken Museth (ken.museth@itn.liu.se)
 *                  3) Ola Nilsson (ola.nilsson@itn.liu.se)
 *************************************************************************************************/

#ifndef __conjugate_gradient_h__
#define __conjugate_gradient_h__


/*! \brief Conjugate gradient iterative solver.
 *
 *  Taken from Sparselib++ modified and made into class.
 *  Uses no preconditioning.
 */
template <class Matrix, class Vector, typename Real>
class ConjugateGradient{
  unsigned int mMaxIter, mIter;
  Real mMaxTolerance, mTolerance;

public:
  ConjugateGradient(unsigned int maxiter, Real tolerance)
  : mMaxIter(maxiter), mIter(0), mMaxTolerance(tolerance), mTolerance(-1) {}
  unsigned int getNumIter() const { return mIter; }
  unsigned int getMaxNumIter() const { return mMaxIter; }
  Real getTolerance() const { return mTolerance; }
  Real getMaxTolerance() const { return mMaxTolerance; }
  /*! The actual algorithm, returns a boolean indicating whether the method converged
   * to the given threshold within the maximum allowed iterations.
   * Use get/set methods above to query statistics.
   */
  bool solve(Matrix &A, Vector& x, const Vector &b){
    Real resid;
    Vector p, q;
    Real alpha, beta, rho, rho_1;
    unsigned int max_iter = mMaxIter;
    Real tol = mMaxTolerance;

    Real normb = norm(b);
    Vector r = b - A*x; // N + M*N*fill

    if (normb == 0.0)
      normb = 1;
    if ((resid = norm(r) / normb) <= tol) { // N
      mTolerance = resid;
      mIter = 0;
      return true;
    }

    for (unsigned int i = 1; i <= max_iter; i++) {
      rho = dot(r, r); // N
      if (i == 1)
        p = r; // N
      else {
        beta = rho / rho_1;
        p = r + beta * p; // N + N + N
      }

      q = A*p; // N + M*N*fill
      alpha = rho / dot(p, q); // N

      x = x + alpha * p; // N + N
      r = r - alpha * q; // N + N

      if ((resid = norm(r) / normb) <= tol) { // N
        mTolerance = resid;
        mIter = i;

        return true;
      }
      rho_1 = rho;
    }

    mTolerance = resid;
    mIter = mMaxIter;
    return false;
  }
};

#endif
