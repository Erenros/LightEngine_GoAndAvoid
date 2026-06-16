#pragma once 

#ifdef _DEBUG
#include <iostream>
#define DEBUG_INFO	std::cout << "[INFO]: "
#define DEBUG_WARN	std::cout << "[WARN]: "
#define DEBUG_ERROR std::cout << "[ERROR]: "
#define ENDL  std::endl;
#define assert( condition ) if ( condition == false ) throw;
#endif // _DEBUG