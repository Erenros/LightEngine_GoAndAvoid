#pragma once 
#include "PrimitiveTypes.h"
#include <string>
#include "Debugger.h"
#include "Vector2.hpp"

using Vector2f = Vector2<float32>;
using Radians = float32;


#ifdef _DEBUG
#define GCLE_NEW new( _NORMAL_BLOCK, __FILE__ , __LINE__ )
#else
#define GCLE_NEW new
#endif

#define assert( condition ) if ( condition == false ) throw;

int8 GenerateRandomNumber(int8 min, int8 max);

#define hundred 0x1
#define thousand 0x2
#define million 0x3	
#define billion 0x4
#define trillion 0x5

std::string NumberToString(float64 value);
std::string RemoveZeroes(std::string string);
std::string ChangeUnit(std::string string, int32 precision);
std::string ChangeUnit(float64 value, int32 precision);

template<typename T>
class SmartPtr 
{
public: 
    SmartPtr(T* p = nullptr) : ptr(p)
    {
        if (p)
            (*refCount)++;
        else
            refCount = nullptr;
    }
     
    SmartPtr(const SmartPtr& other) : ptr(other.ptr), refCount(other.refCount)
    {
        if (refCount)
            ++(*refCount);
    }
     
    SmartPtr& operator=(const SmartPtr& other)
    {
        if (this != &other)
        {
            Release();

            ptr = other.ptr;
            refCount = other.refCount;

            if (refCount)
                ++(*refCount);
        }

        return *this;
    }
     
    ~SmartPtr()
    {
        Release();
    }

    T& operator*() const
    {
        return *ptr;
    }

    T* operator->() const
    {
        return ptr;
    }

    uint64 UseCount() const
    {
        return refCount ? *refCount : 0;
    }

private:
    void Release()
    {
        if (refCount)
        {
            --(*refCount);

            if (*refCount == 0)
            {
                delete ptr;
                delete refCount;
            }
        }
    }

private:
    T* ptr;
    uint64* refCount;
};


struct AABB {
    float32 minX, minY, maxX, maxY;

    bool overlaps(const AABB& other) {
        return minX < other.maxX && minY < other.maxY && maxX > other.minX && maxY > other.minY;
    }

    bool include(const AABB& other) {
        return other.minX >= minX && other.maxX <= maxX && other.minY >= minY && other.maxY <= maxY;
    }
};

AABB GetRotatedAABB(Vector2f center, Vector2f halfSize, Radians rotation);

