#include "GameManager.h"
#include <iostream>
#include "Core/InputManager.h" 
#include "Camera.h"
#include "Entity.h"
#include "Scene.h" 

#include "Shape.h"

GameManager::GameManager(int32 _width, int32 _height) : m_WindW(_width), m_WindH(_height)
{
}

void GameManager::Loop()
{
	isRunning = true;
	 
	Timer time;

	Camera cam;
	cam.Init(mp_window->GetRenderer());
	cam.SetFollowing(m_entities[3]);

	Vector2f screenMiddle = { m_WindW / 2.f, m_WindH / 2.f };

	while (isRunning == true)
	{ 

		InputManager::GetInstance().Update();

		mp_currentScene->OnUpdate();

		time.ResetChrono();

		SDL_RenderClear(mp_window->GetRenderer());
		
		cam.Update();

		for (int i = 0; i < m_entities.size(); i++)
		{
			m_entities[i]->Update(time);

			Shape* realShape = m_entities[i]->GetShape();

			m_entities[i]->SetRenderPosition((realShape->GetPosition() - cam.GetPosition()) * cam.GetZoom() + screenMiddle);

			int mode = 0;

			std::vector<float32> points;

			if (realShape->GetShape() == gcle::Shapes::Rectangle)
			{
				gcle::Rectangle* rect = static_cast<gcle::Rectangle*>(realShape);
				points[0] = rect->GetWidth();
				points[1] = rect->GetHeight();

				mode = 0;
			}

			else if (realShape->GetShape() == gcle::Shapes::Circle)
			{
				gcle::Circle* circ = static_cast<gcle::Circle*>(realShape);
				points[0] = circ->GetRadius();

				mode = 1;
			}

			else if (realShape->GetShape() == gcle::Shapes::Triangle)
			{
				gcle::Triangle* tri = static_cast<gcle::Triangle*>(realShape);
				points[0] = tri->GetTrianglePoints()[0].x;
				points[1] = tri->GetTrianglePoints()[0].y;
				points[2] = tri->GetTrianglePoints()[1].x;
				points[3] = tri->GetTrianglePoints()[1].y;
				points[4] = tri->GetTrianglePoints()[2].x;
				points[5] = tri->GetTrianglePoints()[2].y;

				mode = 2;
			}

			m_entities[i]->SetRenderSize(mode, points);

			mp_window->Draw(m_entities[i]->GetRenderShape());
		}

		SDL_RenderPresent(mp_window->GetRenderer());
	}

	isRunning = false;
}

bool GameManager::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
	{
		std::cout << "SDL_Init_Error :  " << SDL_GetError() << std::endl;
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		std::cout << "Error SDL2_image Initialization";
		return false;
	}

	mp_window = new Window("gcle", m_WindW, m_WindH, SDL_WINDOW_SHOWN, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED);

	if (!mp_window)
	{
		std::cout << "Window Initialization";
		return false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "[Initialisation] : Audio Error : " << SDL_GetError() << std::endl;
		Close();
		return false;
	}

	if (TTF_Init() != 0)
	{
		std::cout << "[Initialisation] : Font Error" << std::endl;
		Close();
		return false;
	} 

	RessourceManager::GetInstance().Init(mp_window->GetRenderer());

	return true;
}

void GameManager::Close()
{
	mp_window->End();

	delete mp_window;

	RessourceManager::GetInstance().DeleteAll();

	Mix_CloseAudio();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	SDL_Quit();
}
