#include "GameManager.h"
#include <iostream>
#include "RessourceManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Camera.h"
#include "Entity.h"

#include "Shape.h"

void GameManager::Loop()
{
	isRunning = true;

	InputManager& IM = InputManager::GetInstance();


	SDL_Renderer* p_renderer = mp_window->GetRenderer();

	// SDL_SetRenderDrawColor(p_renderer, 255, 255, 255, 255);
	// SDL_RenderClear(p_renderer);


	// SDL_Rect rect(50, 50, 50, 50);

	// SDL_SetRenderDrawColor(p_renderer, 0, 0, 0, 255);
	// SDL_RenderDrawRect(p_renderer, &rect);
	// SDL_RenderPresent(p_renderer);

	Camera cam;
	cam.Init(p_renderer);

	// while (true)
	// {
	// 	SDL_Delay(100);
	// 	cam.Update();
	// 	SDL_RenderPresent(p_renderer);
	// }

	/*Rectangle* rectangle = new Rectangle(0, 0, 300, 300, { 250,250, 250, 250 });


	rectangle->SetTexture(tex);*/

	SDL_Texture* tex = RessourceManager::GetInstance().GetTexture("images");

	Triangle* triangle = new Triangle(300.f, 300.f, 500.f, 300.f, 500.f, 500.f, { 255, 255, 255, 255 });
	triangle->SetTexture(tex);


	
	Circle* circle = new Circle(0, 0, 100, 50, { 0, 0, 230, 255 });
	Circle* circle2 = new Circle(200, 200, 100, 4, { 255, 0, 0, 255 });
	Circle* circle3 = new Circle(300, -10, 80, 9, { 255, 255, 0, 255 });
	
	 
	Timer time;

	m_entities.push_back(new Entity());
	m_entities.push_back(new Entity());
	m_entities.push_back(new Entity());

	Transform2D transform;
	Transform2D transform2;
	Transform2D transform3;

	transform.Initialize({ 0.f, 0.f }, 0.f);
	transform2.Initialize({ 200.f, 200.f }, 0.f);
	transform3.Initialize({ 300.f, -10.f }, 0.f);

	m_entities[0]->Initialize(*circle, transform);
	m_entities[1]->Initialize(*circle2, transform2);
	m_entities[2]->Initialize(*circle3, transform3);


	m_entities[0]->SetTag(0);

	cam.SetFollowing(m_entities[0]);


	while (isRunning == true)
	{ 
		time.ResetChrono();

		SDL_RenderClear(mp_window->GetRenderer());
		
		cam.Update();

		for (int i = 0; i < m_entities.size(); i++)
		{
			m_entities[i]->Update(time);

			Vector2f realPos = m_entities[i]->GetPosition();
			Vector2f screenMiddle = { m_WindW / 2.f, m_WindH / 2.f };

			if (cam.GetFollowing() != m_entities[i])
			{
				m_entities[i]->SetRenderPosition(realPos - cam.GetPosition());

			}

			else if (cam.GetFollowing() == m_entities[i])
			{
				m_entities[i]->SetRenderPosition(realPos - cam.GetPosition() + screenMiddle);

			}

			mp_window->Draw(m_entities[i]->GetShape());
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

void GameManager::LaunchGame(int windowWidth, int windowHeight)
{
	m_WindW = windowWidth;
	m_WindH = windowHeight;

	if (!Init()) return;
	
	Loop();
	Close();
}

GameManager* GameManager::Get()
{
	static GameManager* mp_instance = new GameManager; 
	return  mp_instance;
}

