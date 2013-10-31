#ifndef BINFILEREADER_INCLUDED
#define BINFILEREADER_INCLUDED 

#include <iostream>
#include <string>
#include <fstream>
#include <stdint.h>
#include "dataStructs.h"

class binFileReader{
private:
    std::string fName;
    std::fstream ifs;
    bool fileOk;

    binFileReader(const binFileReader& that);
    binFileReader operator=(const binFileReader& that);

public:
    binFileReader(const std::string& filename);

    template<class T>
    bool readRawValue(T& value);
    bool readRawValue(std::string& value, const size_t length);

    bool readDataStruct1(dataStruct1_t& dataStruct);
};


template<class T>
bool binFileReader::readRawValue(T& value){
   if (ifs.eof()){
       return false;
   } else {
       ifs.read( (char*)value, sizeof value);
       return true;
   }
}


#endif
