/*! \class GGL::CoordMatrix
 *
 * \brief A class representing a random sparse matrix
 *
 * This was mainly created for simplicity. I wanted a simple sparse matrix interface which can
 * be used for simple tests for understanding various algorithms. It supports random access
 * through get(), set() or operator(). However, note that operator() directly invokes set()
 * (adding an entry to the matrix). So, random access should only be used for building the
 * matrix structure and setting entries. Otherwise, a more efficient Iterator should be used.
 *
 * In order to add a sequence of entries to the matrix more efficiently, push() followed
 * by endPush() can be used.
 *
 * See CoordMatrix_Test.cpp for usage examples.
 *
 *************************************************************************************************
 *
 * Contributors:
 *                  1) Gunnar Johansson (gunjo@itn.liu.se)
 *
 *************************************************************************************************
 *
 */

#ifndef _COORD_MATRIX
#define _COORD_MATRIX

#include <vector>
#include <deque>
#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <limits>


template <typename Scalar, typename Index>
class CoordMatrix
  {

    protected :

    //! Structure describing an entry in the matrix.
    /*! Stores the row, column and value  components of each entry. */
    struct Entry {
      Entry(Index i, Index j) : row(i), col(j) ,val(0) { }
      Entry(Index i, Index j, Scalar value) : row(i), col(j), val(value) { }
      Index row, col;
      Scalar val;
    };

    //! Determines the type of the data array (std::vector or std::deque)
    typedef std::vector<Entry> DynamicArray;
    //typedef std::deque<Entry> DynamicArray;


    public :

  	typedef Scalar ScalarType;
  	typedef Index IndexType;

    //! Iterator class for CoordMatrix
    /*!
     * An iterator should be used to traverse the data of a coord matrix.
     * You can either traverse the entire matrix or single rows, depending
     * on which begin(), end() pair you choose.
     * \sa begin(), end(), begin(Index), end(Index)
     */
    class Iterator
    {
      friend class CoordMatrix;

      protected :

      Iterator(DynamicArray * data, Index index) : mData(data), mInd(index) { }

      DynamicArray * mData;
      Index mInd;

      public :
      Iterator() : mData(NULL), mInd(0) { }

      //! Returns the row index of the iterator
      inline Index getRow() {
#ifdef NDEBUG
        return (*mData)[mInd].row;
#else
        return mData->at(mInd).row;
#endif
      }
      //! Return the column index of the iterator
      inline Index getCol() {
#ifdef NDEBUG
        return (*mData)[mInd].col;
#else
        return mData->at(mInd).col;
#endif
      }

      //! Advance the iterator one step
      Iterator & operator++() {
        if (mInd < mData->size()) mInd++;
        return *this;
      }
      //! \sa operator++()
      Iterator & operator++(int) { return operator++(); }

      //! Retreat the iterator one step
      Iterator & operator--() {
        if (mInd > 0) mInd--;
        return *this;
      }
      //! \sa operator()
      Iterator & operator--(int) { return operator--(); }

      //! Return a reference to the value
      Scalar & operator*() {
#ifdef NDEBUG
        return (*mData)[mInd].val;
#else
        return mData->at(mInd).val;
#endif
      }
      Scalar operator*() const {
#ifdef NDEBUG
        return (*mData)[mInd].val;
#else
        return mData->at(mInd).val;
#endif
      }

      bool operator!=(const Iterator & iter) const {
        return this->mInd != iter.mInd;
      }

      bool operator==(const Iterator & iter) const {
        return this->mInd == iter.mInd;
      }

    };


    CoordMatrix();
    CoordMatrix(Index rows, Index cols);

    //! Returns the number of rows represented by the matrix
    inline Index getRows() const { return mRows; }

    //! Returns the number of columns represented by the matrix
    inline Index getCols() const { return mCols; }

    //! Resize the matrix
    void resize(Index rows, Index cols);

    //! Reserve memory for a number of non-zero elements
    void reserve(Index elements);

    //! Clear the matrix
    void clear() { mData.clear(); buildRowIndices(); }

    //! Adds en entry to the matrix
    /*! Note that you must call endPush() after any number of pushes
     * in order to rebuild the index data structures */
    inline Scalar & push(Index i, Index j, Scalar val);

    //! Begin a sequence of push operations
    void beginPush() { mIsPushing = true; }

    //! Rebuilds the index data structures after performing push()
    void endPush();

    //! Return an arbitrary element of the matrix
    Scalar get(Index i, Index j);

    //! Return a reference to an arbitrary element (inserts a non-existing entry)
    Scalar & set(Index i, Index j);

    //! \sa get()
    inline Scalar operator()(Index i, Index j) const { return get(i,j); }

    //! \sa set()
    inline Scalar & operator()(Index i, Index j) { return set(i,j); }

    //! Returns a beginning Iterator for the matrix
    Iterator begin() { return Iterator(&mData, 0); }
    const Iterator begin() const { return Iterator(&mData, 0); }

    //! Returns an end Iterator for the matrix
    Iterator end() { return Iterator(&mData, mData.size()); }
    const Iterator end() const { return Iterator(&mData, mData.size()); }

    //! Returns an Iterator starting at the requested row.
    /*! \param row The row index
     */
    Iterator begin(Index row) {
#ifdef NDEBUG
      return Iterator(&mData, mRowInd[row]);
#else
      assert(row < mRows);
      return Iterator(&mData, mRowInd.at(row));
#endif
    }

    //! Returns an Iterator ending at the requested row.
    /*! \param row The row index
     */
    Iterator end(Index row) {
#ifdef NDEBUG
      return Iterator(&mData, mRowInd[row+1]);
#else
      assert(row < mRows);
      return Iterator(&mData, mRowInd.at(row+1));
#endif
    }

    bool isSymmetric(Scalar epsilon = std::numeric_limits<Scalar>::epsilon());

    void printSparse(std::ostream & os);
    void printDense(std::ostream & os);

    template<class Vector, typename Scalar2, typename Index2> friend Vector operator*(CoordMatrix<Scalar2, Index2> &M, const Vector &v);
    protected :

    Index mRows, mCols;
    DynamicArray mData;
    std::vector<Index> mRowInd;
    bool mIsPushing;

    Scalar & insert(Index i, Index j);
    void buildRowIndices();

    struct EntryComparer
    {
      bool operator()( const Entry & a, const Entry & b ) {
        return a.row < b.row || (a.row == b.row && a.col < b.col);
      }
    };

  };

#include "CoordMatrix_Impl.h"

#endif
