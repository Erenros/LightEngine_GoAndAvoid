#pragma once
#include "SDL.h"
#include <vector>
#include <iostream>

class Window;


struct Coord {
	int x;
	int y;
};

class Shape{


protected:

	int m_vertexNbr;
	int m_indicesNbr;

	//Constructors 

	Shape() = default;
	Shape(int vertexNbr, int indicesNbr) :m_vertexNbr(vertexNbr), m_indicesNbr(indicesNbr) {
	};

	virtual void Draw(Window* window) = 0;

};

