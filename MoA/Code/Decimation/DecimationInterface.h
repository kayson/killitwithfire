
#ifndef _DECIMATION_INTERFACE
#define _DECIMATION_INTERFACE

class DecimationInterface
{
public :
  DecimationInterface() { }
  virtual ~DecimationInterface() { }

  virtual bool decimate() = 0;

  virtual bool decimate(unsigned int targetFaces) = 0;


};

#endif
