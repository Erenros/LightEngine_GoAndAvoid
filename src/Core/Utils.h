#pragma once 
#include "PrimitiveTypes.h"


#ifdef _DEBUG
#include <iostream>
#define DEBUG_INFO	std::cout << "[INFO]: "
#define DEBUG_WARN	std::cout << "[WARN]: "
#define DEBUG_ERROR std::cout << "[ERROR]: "
#define ENDL  std::endl;
#define assert( condition ) if ( condition == false ) throw;
#endif // _DEBUG


int8 GenerateRandomNumer(int8 min, int8 max);

#define hundred 0x1
#define thousand 0x2
#define million 0x3
#define billion 0x4
#define trillion 0x5

std::string ChangeUnite(int16 value, short unit, int8 precision);