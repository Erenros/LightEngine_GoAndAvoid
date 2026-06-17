#include "SampleScene.h"

void SampleScene::OnInitialize()
{
	SDL_Texture* tex = RessourceManager::GetInstance().GetTexture("images");

	gcle::Rectangle* rectangle = new gcle::Rectangle(0, 0, 300, 300, { 250,250, 250, 250 });


	rectangle->SetTexture(tex);


	gcle::Triangle* triangle = new gcle::Triangle(0.f, 0.f, 500.f, 300.f, 500.f, 500.f, { 255, 255, 255, 255 });
	triangle->SetTexture(tex);



	gcle::Circle* circle = new gcle::Circle(0, 0, 100, 50, { 0, 0, 230, 255 });

	pEntity = CreateEntity<Entity>(*rectangle);
}

void SampleScene::OnUpdate()
{
	if (InputManager::GetInstance().IsDown('A'))
	{
		DEBUG_INFO << "A" << ENDL;
		pEntity->GoToPosition(500, 500, 200000);
	}
}
