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
#ifndef _HEAP
#define _HEAP

#include <vector>
#include <iostream>
#include <limits>

class Heap
{
public :

  Heap();
  ~Heap();

  struct Heapable
  {
    Heapable() : cost(0), position((std::numeric_limits<unsigned int>::max)()) { }
    Heapable(float val) : cost(val), position((std::numeric_limits<unsigned int>::max)()) { }

    float cost;
    unsigned int position;

    bool operator < (const Heapable & h) const {
      return this->cost < h.cost;
    }
  };

  void push(Heapable * h);

  Heapable * peek();

  Heapable * pop();

  Heapable * remove(Heapable * h);

  inline unsigned int size() { return mNodes.size()-1; }

  inline bool isEmpty() { return size() == 0; }

  void update(Heapable * h);

  void print(std::ostream & os);

protected :

  inline unsigned int parent(unsigned int i) { return i/2; }
  inline unsigned int leftChild(unsigned int i) { return 2*i; }
  inline unsigned int rightChild(unsigned int i) { return 2*i+1; }

  void percolateUp(unsigned int hole);
  void percolateDown(unsigned int hole);

  std::vector<Heapable *> mNodes;
};

#endif
