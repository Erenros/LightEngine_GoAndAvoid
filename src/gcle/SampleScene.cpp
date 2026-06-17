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
	Vector2f pos = pEntity->GetPosition(0.5f, 0.5f);

	if ((InputManager::GetInstance().IsHeld('D')))
	{
		pEntity->SetPosition(pos.x + 1, pos.y);
		pos = pEntity->GetPosition();
	}

	if (InputManager::GetInstance().IsHeld('Q'))
	{
		pEntity->SetPosition(pos.x - 1, pos.y);
		pos = pEntity->GetPosition();
	}

	if (InputManager::GetInstance().IsHeld('S'))
	{
		pEntity->SetPosition(pos.x, pos.y + 1);
		pos = pEntity->GetPosition();
	}

	if (InputManager::GetInstance().IsHeld('Z'))
	{
		pEntity->SetPosition(pos.x, pos.y - 1);
		pos = pEntity->GetPosition();
	}
}
