#pragma once
#include "Shape.h"

class Triangle: public Shape{

	SDL_Vertex m_vertices[3];

public:

	//Contructors

	Triangle() = default;
	Triangle(int x1, int y1, int x2, int y2, int x3, int y3, SDL_Color color){


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

