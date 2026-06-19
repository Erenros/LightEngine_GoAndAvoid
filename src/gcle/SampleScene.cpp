#include "SampleScene.h"

void SampleScene::OnInitialize()
{
	SDL_Texture* tex = RessourceManager::GetInstance().GetTexture("images");

	gcle::Rectangle* rectangle = new gcle::Rectangle(0, 0, 100, 100, { 250,250, 250, 250 });
	gcle::Rectangle* rectangle2 = new gcle::Rectangle(200, 200, 100, 100, { 250,250, 250, 250 });
	gcle::Rectangle* rectangle3 = new gcle::Rectangle(200, 50, 100, 100, { 250,250, 250, 250 });


	rectangle->SetTexture(tex);
	rectangle2->SetTexture(tex);


	gcle::Triangle* triangle = new gcle::Triangle(0.f, 0.f, 500.f, 300.f, 500.f, 500.f, { 255, 255, 255, 255 });
	triangle->SetTexture(tex);



	gcle::Circle* circle = new gcle::Circle(200, 0, 50, 50, { 0, 0, 230, 255 });
	gcle::Circle* circle2 = new gcle::Circle(400, 200, 100, 50, { 0, 0, 230, 255 });
	gcle::Circle* circle3 = new gcle::Circle(200, 200, 100, 50, { 0, 0, 230, 255 });

	pEntity = CreateEntity<Entity>(*rectangle);
	pEntity->SetRigidBody(true);
	Entity* entity1 = CreateEntity<Entity>(*circle2);
	entity1->SetRigidBody(true);
	Entity* entity2 = CreateEntity<Entity>(*circle3);
	entity2->SetRigidBody(true);
}

void SampleScene::OnUpdate()
{
	Vector2f pos = pEntity->GetPosition();

	if (InputManager::GetInstance().IsHeld('Z'))
	{ 
		pEntity->SetPosition(pos.x, pos.y - 1);
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

	if (InputManager::GetInstance().IsHeld('D'))
	{
		pEntity->SetPosition(pos.x + 1, pos.y);
		pos = pEntity->GetPosition();
	}
}
