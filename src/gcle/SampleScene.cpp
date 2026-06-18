#include "SampleScene.h"

void SampleScene::OnInitialize()
{
	SDL_Texture* tex = RessourceManager::GetInstance().GetTexture("images");

	gcle::Rectangle* rectangle = new gcle::Rectangle(0, 0, 300, 300, { 250,250, 250, 250 });


	rectangle->SetTexture(tex);


	gcle::Triangle* triangle = new gcle::Triangle(100.f, 100.f, 600.f, 400.f, 600.f, 600.f, { 255, 0, 0, 255 });
	triangle->SetTexture(tex);



	gcle::Circle* circle = new gcle::Circle(0, 0, 100, 50, { 0, 0, 230, 255 });


	Circle* circle2 = new Circle(300, 300, 80, 50, { 255, 255, 0, 255 });

	pEntity = CreateEntity<Entity>(*rectangle);
	pEntity = CreateEntity<Entity>(*triangle);
	pEntity = CreateEntity<Entity>(*circle);
	pEntity = CreateEntity<Entity>(*circle2);



	
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
