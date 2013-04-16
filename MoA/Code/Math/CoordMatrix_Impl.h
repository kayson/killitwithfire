

template <typename Scalar, typename Index>
CoordMatrix<Scalar, Index>::CoordMatrix() : mIsPushing(false)
{
  this->resize(0,0);
}

template <typename Scalar, typename Index>
CoordMatrix<Scalar, Index>::CoordMatrix(Index rows, Index cols) : mIsPushing(false)
{
  this->resize(rows, cols);
}


template <typename Scalar, typename Index>
  void CoordMatrix<Scalar, Index>::resize(Index rows, Index cols)
{
	bool crop = (rows < mRows || cols < mCols);
	mRows = rows;
	mCols = cols;
	mRowInd.resize(rows+1);

	if (crop) {
		// Remove entries falling outside
		typename DynamicArray::iterator iter = mData.begin();
		typename DynamicArray::iterator iend = mData.end();
		while (iter != iend) {
			if ((*iter).row >= rows || (*iter).col >= cols) {
  				iter = mData.erase(iter);
  				iend = mData.end(); // iend apparently needs to be reassigned
			}
			else
				iter++;
		}

		// Rebuild the row indices
		buildRowIndices();
	}
}


template <typename Scalar, typename Index>
void CoordMatrix<Scalar, Index>::reserve(Index elements)
{
  mData.reserve(elements);
}


template <typename Scalar, typename Index>
  Scalar & CoordMatrix<Scalar, Index>::push(Index i, Index j, Scalar val)
{
	#ifndef NDEBUG
		// Check range and for already existing entry in debug
		assert(i >= 0 && i < mRows && j >= 0 && j < mCols);
		typename DynamicArray::iterator iter = mData.begin();
		typename DynamicArray::iterator iend = mData.end();
		while (iter != iend) {
			assert(!((*iter).row == i && (*iter).col == j));
			iter++;
		}
	#endif
	mData.push_back(Entry(i,j,val));
  return mData[mData.size()-1].val;
}

template <typename Scalar, typename Index>
  void CoordMatrix<Scalar, Index>::endPush()
{
  mIsPushing = false;

	// Sort the data in row-major order
	std::sort(mData.begin(), mData.end(), EntryComparer());

	// Rebuild the row indices
	buildRowIndices();
}


template <typename Scalar, typename Index>
  Scalar CoordMatrix<Scalar, Index>::get(Index i, Index j)
{
  assert(!mIsPushing && i >= 0 && i < mRows && j >= 0 && j < mCols);

  Iterator iter = this->begin(i);
  Iterator iend = this->end(i);

  while (iter != iend) {
    if (iter.getCol() == j)
      return (*iter);
    iter++;
  }

  return 0;
}


template <typename Scalar, typename Index>
  Scalar & CoordMatrix<Scalar, Index>::set(Index i, Index j)
{
  if (mIsPushing)  return push(i, j, 0);

  assert(i >= 0 && i < mRows && j >= 0 && j < mCols);

  Iterator iter = this->begin(i);
  Iterator iend = this->end(i);

  while (iter != iend) {
    if (iter.getCol() == j)
      return (*iter);
    iter++;
  }

  return insert(i,j);
}



template <typename Scalar, typename Index>
  bool CoordMatrix<Scalar, Index>::isSymmetric(Scalar epsilon)
{
  if (mRows != mCols) return false;

  Iterator iter = this->begin();
  Iterator iend = this->end();

  while (iter != iend) {
    Index row = iter.getRow();
    Index col = iter.getCol();
    if (row != col && std::fabs(this->get(col, row) - (*iter)) > epsilon)
      return false;
    iter++;
  }

  return true;
}



template <typename Scalar, typename Index>
  void CoordMatrix<Scalar, Index>::printSparse(std::ostream & os)
{
  if (mData.size() == 0) {
    os << "[ empty matrix ]" << std::endl;
    return;
  }

  typename DynamicArray::iterator iter = mData.begin();
  typename DynamicArray::iterator iend = mData.end();

  // os << "[" << std::endl;
  while (iter != iend) {
    // os << " (" << (*iter).row << "," << (*iter).col << ") -> " << (*iter).val << std::endl;
    os << (*iter).row << " " << (*iter).col << " " << (*iter).val << std::endl;
    iter++;
  }
  //os << "]" << std::endl;
}


template <typename Scalar, typename Index>
  void CoordMatrix<Scalar, Index>::printDense(std::ostream & os)
{
  typename DynamicArray::iterator iter = mData.begin();
  typename DynamicArray::iterator iend = mData.end();

  os << "[ ";
  for (Index row = 0; row < mRows; row++) {
    for (Index col = 0; col < mCols; col++) {
      if (iter != iend && (*iter).row == row && (*iter).col == col) {
	os << '\t' << (*iter).val;
	iter++;
      }
      else
	os << '\t' << 0;
    }
    os << std::endl;
  }
  os << "]" << std::endl;
}




template <typename Scalar, typename Index>
  Scalar & CoordMatrix<Scalar, Index>::insert(Index i, Index j)
{
  typename DynamicArray::iterator iter = mData.begin();
  typename DynamicArray::iterator iend = mData.end();

  // Find the position of the entry (sorted lexicographically)...
  while (iter != iend &&
	 ((*iter).row < i ||
	  (*iter).row == i && (*iter).col < j))
    iter++;

  // ...and insert
  iter = mData.insert(iter, Entry(i,j));

  // Shift the row indices
  for (Index row = i+1; row <= mRows; row++)
    mRowInd[row]++;

  return (*iter).val;
}


template <typename Scalar, typename Index>
  void CoordMatrix<Scalar, Index>::buildRowIndices()
{
  typename DynamicArray::iterator iter = mData.begin();
  typename DynamicArray::iterator iend = mData.end();
  Index row = 0, count = 0;
  while (iter != iend) {
    while (row < (*iter).row) {
      mRowInd[row] = count;
      row++;
    }

    mRowInd[row] = count;
    while (iter != iend && (*iter).row == row) {
      iter++;
      count++;
    }
    row++;
  }

  while (row <= mRows) {
    mRowInd[row] = count;
    row++;
  }
}

template <class Vector, typename Scalar, typename Index>
Vector operator*(CoordMatrix<Scalar, Index> &M, const Vector &v){
  assert(M.mCols == v.size());
  Vector result(M.mCols);

  typename CoordMatrix<Scalar, Index>::Iterator iter = M.begin();
  typename CoordMatrix<Scalar, Index>::Iterator iend = M.end();

  while (iter != iend) {
    Index row = iter.getRow();
    Scalar sum = 0;
    while (iter != iend && row == iter.getRow()) {
      sum += (*iter) * v[iter.getCol()];
      iter++;
    }
    result[row] = sum;
  }
  return result;
}
