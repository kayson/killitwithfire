#ifndef CONSTANTVALUEEXTRAPOLATION_H
#define CONSTANTVALUEEXTRAPOLATION_H

#include "Extrapolation.h"
#include "GridField.h"

template<class T>
class ConstantValueExtrapolation  : public Extrapolation<T>
{
private:
	T _value;
public:
	ConstantValueExtrapolation(const T &value = T()) : _value(value) {}

    virtual ~ConstantValueExtrapolation(){}

	virtual Extrapolation<T>* clone() const
	{
		return new ConstantValueExtrapolation<T>();
	}

	virtual T doExtrapolation(const GridField<T>& g, int i, int j, int k) const
	{
		return _value;
	}
};

#endif
