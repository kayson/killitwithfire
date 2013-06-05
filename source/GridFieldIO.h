//
//  GridFiledIO.h
//  FluidFire
//
//  Created by Johannes Deligiannis on 5/23/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FluidFire__GridFiledIO__
#define __FluidFire__GridFiledIO__

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include "GridField.h"

/**
    Filformat:
 
    int xsize
    int ysize
    int zsize
    glm::dmat4x4 _trans
    int datasize
    T _data
*/

class Data {
public:
    
};

class GridFieldIO{
private:
    const int digits_;
    std::string prefix_;
protected:
    GridFieldIO(int digits = 3, const std::string &prefix = "GridData_"):digits_(digits),prefix_(prefix){}
public:
    void setPrefix(std::string prefix);
    std::string filename(int n) const;
    
    template<class T>
    static void save(const GridField<T> &field, std::ofstream &stream);

    template<class T>
    static GridField<T> load(std::ifstream &stream);
};

/**
    Sparar en sekvens av GridFields, blir på formen prefixXXX
        
    tex: Levelset_001.bin ,Levelset_002.bin ,Levelset_003.bin ... etc
*/

class GridFieldSequenceCreator : public GridFieldIO {
private:
    int current_;
    int metafields_;
    const std::string metafile_;
    const std::string filename_;
protected:
    bool init();
    std::fstream meta_;
    
    std::string temp_filename(int n) const;
public:
    GridFieldSequenceCreator(const std::string fname):current_(0),metafields_(0),metafile_("tmp-meta.vol"),filename_(fname){
        init();
    }
    /**
        Lägg till info om sekvensen. Måste göras innan sekvensen fått in första värdet,
        tex :  float dt = 0.1f;
                addMeta("dt",&dt,1);
    */
    bool addMeta(const std::string &key, float value);

    template<class T>
    bool addMeta(const std::string &key, T value);

    /**
        Lägger till ett gridfield till sekvensen
    */
    template<class T>
    void addField(const GridField<T> &field);
    
    /**
        Avslutar sekvensen - komprimerar alla delfiler till en slutgitlig fil
     */
    bool finalize();
};

/**
    Inputfiler måste vara på formen prefixXXX
    tex: Levelset_001,Levelset_002.bin,Levelset_003.bin ... etc
 */
class GridFieldSequenceLoader : public GridFieldIO {
private:
    std::ifstream in;
    std::string filename_;
    mutable int current_;
    std::map<std::string, float> meta_;
    int grid_count_;
protected:
    bool init() const;
public:
    GridFieldSequenceLoader(const std::string fname);

    //Get the number of grids stored in file
    int grid_count() const;
    
    //Get a data from meta field
    float getMeta(const std::string &s) const;
    
    //Get a gridfield
    template<class T>
    GridField<T> getNext();
    
    friend std::ostream& operator<<(std::ostream &o, const GridFieldSequenceLoader &seq);
    
};

#endif /* defined(__FluidFire__GridFiledIO__) */







