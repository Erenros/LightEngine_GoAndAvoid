#pragma once 
#include "PrimitiveTypes.h"
#include <string>
#include "Debugger.h"

int8 GenerateRandomNumer(int8 min, int8 max);

#define hundred 0x1
#define thousand 0x2
#define million 0x3
#define billion 0x4
#define trillion 0x5

std::string NumberToString(float64 value);
std::string RemoveZeroes(std::string string);
std::string RemoveZeroes(float64 value);
std::string ChangeUnit(std::string string, int32 precision);
std::string ChangeUnit(float64 value, int32 precision);

template<typename T>
class SmartPtr {
	T* ptr;
	uint32* ref_count;

public:

	SmartPtr() : ptr(nullptr), ref_count(nullptr) { }
	SmartPtr(const SmartPtr& other) { ptr = other.ptr; ref_count = other.ref_count; (*ref_count)++; }
	~SmartPtr() { Destroy(); }

	void New() { 
		ptr = new T(); 
		ref_count = new int32(); 
		ref_count = 1; 
	}

	void Destroy() {
		if (ptr == nullptr)
			return;

		(*ref_count)--;
		if(*ref_count <= 0)
		{
			delete ptr;
			delete ref_count;
			ptr = nullptr;
			ref_count = nullptr;
		}
	}

	T& operator -> () { return *ptr; }

};
