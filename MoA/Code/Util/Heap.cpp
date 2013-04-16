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
#include "Heap.h"
#include <cassert>

Heap::Heap()
{
  // Add a sentinel (dummy node) for convinience
  mNodes.push_back(new Heapable(-(std::numeric_limits<float>::max)()));
}

Heap::~Heap()
{
  // delete sentinel node
  delete mNodes.front();
}


void Heap::push(Heapable * h)
{
  mNodes.push_back(h);
  percolateUp(mNodes.size()-1);
}


Heap::Heapable * Heap::peek()
{
  return (mNodes.size() > 1 ? mNodes[1] : NULL);
}


Heap::Heapable * Heap::pop()
{
  if (mNodes.size() == 1)  return NULL;
  return remove(mNodes[1]);
}


Heap::Heapable * Heap::remove(Heapable * h)
{
  assert(h->position != (std::numeric_limits<unsigned int>::max)() && h->position < mNodes.size());

  unsigned int hole = h->position;
  mNodes[hole] = mNodes[mNodes.size()-1];
  mNodes.pop_back();
  h->position = (std::numeric_limits<unsigned int>::max)();

  if (hole == mNodes.size()) return h;

  if (*mNodes[hole] < *h)
    percolateUp(hole);
  else
    percolateDown(hole);

  return h;
}


void Heap::update(Heapable * h)
{
  assert(h->position != (std::numeric_limits<unsigned int>::max)() && h->position < mNodes.size());

  if (*h < *mNodes[parent(h->position)])
    percolateUp(h->position);
  else
    percolateDown(h->position);
}


void Heap::print(std::ostream & os)
{
  std::vector<Heapable *>::iterator iter = mNodes.begin();
  std::vector<Heapable *>::iterator iend = mNodes.end();
  ++iter; // skip the sentinel
  while (iter != iend) {
    os << (*iter)->cost << "(" << (*iter)->position << ") ";
    ++iter;
  }
  os << std::endl;
}


void Heap::percolateUp(unsigned int hole)
{
  Heapable * start = mNodes[hole];
  while (*start < *mNodes[ parent(hole) ]) {
    mNodes[hole] = mNodes[ parent(hole) ];
    mNodes[hole]->position = hole;
    hole = parent(hole);
  }
  mNodes[hole] = start;
  start->position = hole;
}

void Heap::percolateDown(unsigned int hole)
{
  Heapable * start = mNodes[hole];
  unsigned int currentSize = mNodes.size();

  while (leftChild(hole) < currentSize) {
    unsigned int left = leftChild(hole);
    unsigned int right = rightChild(hole);
    unsigned int child = left;
    if (right < currentSize && *mNodes[right] < *mNodes[left])
      child = right;

    if (*mNodes[child] < *start) {
      mNodes[hole] = mNodes[child];
      mNodes[hole]->position = hole;
    }
    else break;

    hole = child;
  }
  mNodes[hole] = start;
  start->position = hole;
}
