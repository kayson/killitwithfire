//
//  GridFiledIO.cpp
//  FluidFire
//
//  Created by Johannes Deligiannis on 5/23/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#include "GridFieldIO.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <iterator>
#include <typeinfo>
#include "GridField.hpp"

void GridFieldIO::setPrefix(std::string prefix){
    prefix_ = prefix;
}

std::string GridFieldIO::filename(int n) const{
    std::stringstream ss;
    ss << prefix_;
    ss.width(digits_);
    ss.fill('0');
    ss << n << ".vol";
    return ss.str();
}

template<class T>
void GridFieldIO::save(const GridField<T> &field, std::ofstream &stream){
    stream.write((char *)&(field._xdim), sizeof(int));
    stream.write((char *)&(field._ydim), sizeof(int));
    stream.write((char *)&(field._zdim), sizeof(int));
    int sizeofT = sizeof(T);
    stream.write((char *)&(sizeofT), sizeof(int));
    int count = field.cellCount();
    stream.write((char *)(field._data), sizeofT*count);
}

template<class T>
GridField<T> GridFieldIO::load(std::ifstream &stream){
    
    GridField<T> field(1,1,1,10, new ConstantValueExtrapolation<T>(0));

    int xdim,ydim,zdim;
    stream.read((char *)&(xdim), sizeof(int));
    stream.read((char *)&(ydim), sizeof(int));
    stream.read((char *)&(zdim), sizeof(int));
    field.set_xdim(xdim);
    field.set_ydim(ydim);
    field.set_zdim(zdim);
    
    //Allocate data 
    int count = field.cellCount();
    field._data = new T[count];
    
    //Storlek på varje element
    int sizeofT;
    stream.read((char *)&(sizeofT), sizeof(int));
    
    //Läs in alla element
    char *tmp =  new char[count*sizeofT];
    stream.read(tmp, sizeofT*count);
    
    //Convertera ifall datatypen skiljer, tex om det är sparat som float, så kan man ladda som double =)
    for (int i = 0; i < count; i++) field._data[i] = static_cast<T>(tmp[i]);
    delete [] tmp;
    return field;
}

std::string GridFieldSequenceCreator::temp_filename(int n) const{
    std::stringstream ss;
    ss << "tmp-" << GridFieldIO::filename(n);
    return ss.str();
}


bool GridFieldSequenceCreator::init(){
    //Create meta file
    meta_.open(metafile_.c_str(),(std::ios::binary
                                  | std::ios::trunc
                                  | std::ios::in
                                  | std::ios::out));

    return meta_.good();
}


bool GridFieldSequenceCreator::addMeta(const std::string &s, float value){
    unsigned long k_n = s.length();
    meta_.write((char*)&k_n, sizeof(unsigned long));             //Write length of key
    meta_.write(s.c_str(), sizeof(char)*k_n);                        //Write key
    meta_.write((char*)&value, sizeof(float));                   //Write value
    metafields_++;

    return meta_.good();
}


template<class T>
void GridFieldSequenceCreator::addField(const GridField<T> &field){
    std::string filename = temp_filename(current_++);
    std::ofstream out(filename.c_str(),std::ios::binary);
    if (out){
        GridFieldIO::save(field,out);
    }else{
        current_--;
        remove(filename.c_str());
    }
    out.close();
}

bool GridFieldSequenceCreator::finalize(){

    //Finalize metafile
    addMeta("-s",static_cast<float>(current_));
    
    //Create the resulting sequence file
    std::ofstream o(filename_.c_str(),std::ios::binary | std::ios::trunc);
    o.write((char*)&metafields_, sizeof(int));

    //Write meta
    //Get size of file
    meta_.seekg (0,meta_.end);
    long size = meta_.tellg();
    meta_.seekg (0);
    // allocate memory for file content
    char *buffer = new char[size];
    // read content of infile
    meta_.read(buffer,size*sizeof(char));
    // write to outfile
    o.write(buffer,size*sizeof(char));
    meta_.close();
    delete [] buffer;
    
    //Write fields
    for (int i = 0; i < current_; i++) {
        std::ifstream f(temp_filename(i).c_str(),std::ios::binary);
        if (f) {
            // get size of file
            f.seekg (0,f.end);
            long size = f.tellg();
            f.seekg (0,f.beg);
            // allocate memory for file content
            char *buffer = new char[size];
            // read content of infile
            f.read(buffer,size*sizeof(char));
            // write to outfile
            o.write(buffer,size*sizeof(char));
            f.close();
            delete [] buffer;
        }
    }
    o.close();

    //Everything ok - remove temporary files
    remove(metafile_.c_str());
    for (int i = 0; i < current_; i++) {
        remove(temp_filename(i).c_str());
    }
    return true;
}

GridFieldSequenceLoader::GridFieldSequenceLoader(const std::string fname):filename_(fname),current_(0){
    in.open(fname.c_str(), std::ios::binary);
    if (in) {
        int n;
        in.read((char*)&n, sizeof(int));
        for (int i = 0; i < n; i++) {
            unsigned long k_n;
            in.read((char*)&k_n, sizeof(unsigned long));
            char key[k_n];
            in.read(key, sizeof(char)*k_n);
            float value;
            in.read((char*)&value, sizeof(float));
            meta_.insert(std::pair<std::string, float>(std::string(key,k_n),value));
        }
    }
}

int GridFieldSequenceLoader::grid_count() const{
    return grid_count_;
}
float GridFieldSequenceLoader::getMeta(const std::string &s) const{
    return ((*meta_.find(s)).second);
}

template <class T>
GridField<T> GridFieldSequenceLoader::getNext(){
    return GridFieldIO::load<T>(in);
}

std::ostream& operator<<(std::ostream &o, const GridFieldSequenceLoader &seq){
    o <<"\"" << seq.filename_  << "\"";
    typedef std::map<std::string, float>::const_iterator iterator;
    for (iterator it = seq.meta_.begin(); it != seq.meta_.end(); it++) {
        o << it->first << " : " << it->second << std::endl;
    }
    
    return o;
}














