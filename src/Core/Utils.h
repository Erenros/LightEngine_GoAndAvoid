#pragma once 

#ifdef _DEBUG
#include <iostream>
#define DEBUG_INFO	std::cout << "[INFO]: "
#define DEBUG_WARN	std::cout << "[WARN]: "
#define DEBUG_ERROR std::cout << "[ERROR]: "
#define ENDL  std::endl;
#define assert( condition ) if ( condition == false ) throw;
#define assert_log( condition, message ) if ( condition == false ) throw DEBUG_ERROR << message;
#endif // _DEBUG