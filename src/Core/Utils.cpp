#include "Utils.h"
#include <string>

int8 GenerateRandomNumer(int8 min, int8 max){
	return (rand() % max) + min;
}

//std::string ChangeUnite(int16 value, short unit, int8 precision){
//	std::string sValue = std::to_string(value);
//	std::string result = "";
//
//	//int8 dot = unit * 3;
//	//std::distance(sValue.begin(), dot);
//	//sValue.insert('.');
//
//
//}
