#pragma once
#include "SDL.h"
#include <vector>
#include <iostream>
#include "Engine/Window.h"

//TODO enlever ca
class Coord {
public:
	int x;
	int y;
};

enum class Shapes {
	Triangle,
	Rectangle,
	Circle
};



class Shape {

	SDL_Texture* m_texture = nullptr;
	



protected:

	

	//circle
	int m_radius;
	Coord m_center;


	//rectangle
	int m_height;
	int m_width;
	Coord m_coordinates;






	Shapes m_shapes;


	int m_vertexNbr;
	int m_indicesNbr;

	//Constructors 

	Shape() = default;
	Shape(int vertexNbr, int indicesNbr) :m_vertexNbr(vertexNbr), m_indicesNbr(indicesNbr) {};

	virtual void Draw(Window* window) = 0;


public:
	//Getters 

	virtual float GetRadius() { return 0.f; };
	virtual float GetHeight() { return 0.f; };
	virtual float GetWidth() { return 0.f; };
	virtual Coord GetCenter() { return { 0, 0 }; };
	virtual Coord GetCoordinates() { return { 0, 0 }; };

	SDL_Texture* GetTexture() { return m_texture; };

	//Setters 

	virtual void SetRadius(int radius) {};
	virtual void SetHeight(float height) {};
	virtual void SetWidth(float width) {};
	virtual void SetCenter(Coord center) {};
	virtual void SetCoordinates(Coord coordinates) {};
	
	void SetTexture(SDL_Texture* tex) {
		m_texture = tex;
	}

};



class Rectangle : public Shape {

	SDL_Vertex m_vertices[4];

	//Constructors

	const int m_rectangleIndices[6] = {
		0,1,2,
		2,3,1
	};

public:
	Rectangle(int x, int y, float height, float width, SDL_Color color) {
		m_shapes = Shapes::Rectangle;

		m_height = height;
		m_width = width;

		SDL_Vertex vertex1 = { {x, y}, color, {1, 1} };
		SDL_Vertex vertex2 = { {x + m_width, y}, color, {1, 1} };
		SDL_Vertex vertex3 = { {x, y + m_height}, color, {1, 1} };
		SDL_Vertex vertex4 = { {x + m_width, y + m_height}, color, {1, 1} };

		m_vertices[0] = vertex1;
		m_vertices[1] = vertex2;
		m_vertices[2] = vertex3;
		m_vertices[3] = vertex4;

		m_indicesNbr = 6;
		m_vertexNbr = 4;

	}


	//Getters

	float GetHeight() override {
		return m_height;
	}
	float GetWidth() override {
		return m_width;
	}
	Coord GetCoordinates() override {
		return m_coordinates;
	}


	//Setters

	void SetHeight(float height) override {
		m_height = height;
	}
	void SetWidth(float width) override {
		m_width = width;
	}
	void SetCoordinates(Coord coordinates) override {
		m_coordinates = coordinates;
	}



	void Draw(Window* window);

};

class Triangle : public Shape {

	SDL_Vertex m_vertices[3];

public:

	//Contructors

	Triangle(int x1, int y1, int x2, int y2, int x3, int y3, SDL_Color color) {
		m_shapes = Shapes::Triangle;


		m_indicesNbr = 0;
		m_vertexNbr = 3;

		SDL_Vertex vertex1 = { {x1, y1}, color, {1, 1} };
		SDL_Vertex vertex2 = { {x2, y2}, color, {1, 1} };
		SDL_Vertex vertex3 = { {x3, y3}, color, {1, 1} };

		m_vertices[0] = vertex1;
		m_vertices[1] = vertex2;
		m_vertices[2] = vertex3;
	}

	void Draw(Window* pWindow) override;
};

