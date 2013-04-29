// Johan Noren
// Abstrakt bas-klass for olika extrapolerings-implementeringar

#ifndef EXTRAPOLATION_H
#define EXTRAPOLATION_H

template<class T> class GridField;

template<class T>
class Extrapolation{
protected:
    //Implementera i subklasser
    virtual T doExtrapolation(const GridField<T>& g, int i, int j, int k) const = 0;
public:
    virtual ~Extrapolation(){}
	virtual Extrapolation<T>* clone() const = 0;
	T extrapolate(const GridField<T>& g, int i, int j, int k) const{
        //Stäng av extrapolation
        GridField<T> *grid = const_cast<GridField<T> *>(&g);
        grid->setExtrapolation(nullptr);
        
        //Anropa implementation i subklass...
        T val = doExtrapolation(*grid, i, j, k);

        //Slå på extrapolation igen
        grid->setExtrapolation(this); 
        return val;
    }
};

#endif