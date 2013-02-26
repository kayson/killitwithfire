//
//  GridFieldFileManager.h
//  FuidFire
//
//  Created by Johannes Deligiannis on 2/23/13.
//  Copyright (c) 2013 Johannes Deligiannis. All rights reserved.
//

#ifndef __FuidFire__GridFieldFileManager__
#define __FuidFire__GridFieldFileManager__
#include "GridField.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "helper.h"
class GridFieldFileManager {
public:
    template<class T>
    static void readFromFile(GridField<T> &field,std::string name ){
        /*
        name = name.append(".bin"); //Filnamn
        std::ifstream file (name, std::ios::in | std::ios::binary);
        file.seekg (0);
        file.read ((char*)&field, sizeof (GridField<T>)); //Läs in GridField<T>
        file.read((char*)(field._interpolation), sizeof(*field._interpolation)); //Interpolation är en pekare så den måste läsas in separat
        T * data = new T[field.cellCount()]; //Precis som interpolationen måste datan läsas in separat
        file.read((char*)data, field.cellCount()*sizeof(T));
        std::cout << file.tellg() << std::endl;
        field._data = data;
         */

    }
    
    template<class T>
    static bool writeToFile(const GridField<T> &field,std::string name ){
        /*name = name.append(".bin"); //Filnamn
        std::fstream file(name, std::ios::out | std::ios::binary);
        file.seekp(0);
        file.write((char*)(&field), sizeof(GridField<T>));
        //Interpolation
        file.write((char*)(field._interpolation), sizeof(*field._interpolation));
        std::cout << file.tellp() << std::endl;
        file.write((char*)field._data, field.cellCount()*sizeof(T)); //Write the dynamic array
        std::cout << file.tellp() << std::endl;

        file.close();
         */
        return true;
    }

};
#endif /* defined(__FuidFire__GridFieldFileManager__) */
